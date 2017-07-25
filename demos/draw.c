#include <gral.h>
#define _USE_MATH_DEFINES
#include <math.h>
#define ROUNDED_CORNER_RATIO .5522847f

struct gral_demo {
	struct gral_application *application;
	struct gral_window *window;
};

static int close(void *user_data) {
	return 1;
}

static void rounded_corner_x(struct gral_draw_context *draw_context, float x, float y) {
	float current_x, current_y;
	gral_draw_context_get_current_point(draw_context, &current_x, &current_y);
	gral_draw_context_curve_to(draw_context, current_x + ROUNDED_CORNER_RATIO * (x - current_x), current_y, x, y + ROUNDED_CORNER_RATIO * (current_y - y), x, y);
}

static void rounded_corner_y(struct gral_draw_context *draw_context, float x, float y) {
	float current_x, current_y;
	gral_draw_context_get_current_point(draw_context, &current_x, &current_y);
	gral_draw_context_curve_to(draw_context, current_x, current_y + ROUNDED_CORNER_RATIO * (y - current_y), x + ROUNDED_CORNER_RATIO * (current_x - x), y, x, y);
}

static void add_circle(struct gral_draw_context *draw_context, float x, float y, float size) {
	float radius = size / 2.f;
	gral_draw_context_move_to(draw_context, x, y + radius);
	rounded_corner_y(draw_context, x + radius, y);
	rounded_corner_x(draw_context, x + size, y + radius);
	rounded_corner_y(draw_context, x + radius, y + size);
	rounded_corner_x(draw_context, x, y + radius);
}

static void add_rounded_rectangle(struct gral_draw_context *draw_context, float x, float y, float width, float height, float radius) {
	gral_draw_context_move_to(draw_context, x, y+radius);
	rounded_corner_y(draw_context, x + radius, y);
	gral_draw_context_line_to(draw_context, x + width - radius, y);
	rounded_corner_x(draw_context, x + width, y + radius);
	gral_draw_context_line_to(draw_context, x+width, y+height-radius);
	rounded_corner_y(draw_context, x + width - radius, y + height);
	gral_draw_context_line_to(draw_context, x+radius, y+height);
	rounded_corner_x(draw_context, x, y + height - radius);
	gral_draw_context_close_path(draw_context);
}

static void add_star(struct gral_draw_context *draw_context, float x, float y, float size) {
	float radius = size / 2.f;
	float cx = x + radius;
	float cy = y + radius;
	float a = (float)M_PI * 3.f / 2.f;
	gral_draw_context_move_to(draw_context, cx, y);
	int i;
	for (i = 1; i < 5; i++) {
		a += (float)M_PI * 2.f * 2.f / 5.f;
		gral_draw_context_line_to(draw_context, cx + cosf(a)*radius, cy + sinf(a)*radius);
	}
	gral_draw_context_close_path(draw_context);
}

static void draw(struct gral_draw_context *draw_context, void *user_data) {
	gral_draw_context_add_rectangle(draw_context, 10.f, 10.f, 180.f, 180.f);
	gral_draw_context_fill(draw_context, 1.f, 0.f, 0.f, 1.f);
	gral_draw_context_add_rectangle(draw_context, 210.f, 10.f, 180.f, 180.f);
	gral_draw_context_stroke(draw_context, 3.f, 0.f, 0.f, 1.f, 1.f);
	gral_draw_context_add_rectangle(draw_context, 410.f, 10.f, 180.f, 180.f);
	struct gral_gradient_stop stops[] = {
		{0.f, 1.f, 0.f, 0.f, 1.f},
		{.3f, 1.f, 1.f, 0.f, 1.f},
		{1.f, 0.f, 1.f, 0.f, 1.f}
	};
	gral_draw_context_fill_linear_gradient(draw_context, 410.f, 10.f, 590.f, 100.f, stops, 3);
	add_circle(draw_context, 10.f, 210.f, 180.f);
	add_rounded_rectangle(draw_context, 210.f, 210.f, 180.f, 180.f, 20.f);
	add_star(draw_context, 410.f, 210.f, 180.f);
	gral_draw_context_fill(draw_context, 0.f, 0.f, 1.f, 1.f);
}

static void resize(int width, int height, void *user_data) {

}

static void mouse_enter(void *user_data) {

}

static void mouse_leave(void *user_data) {

}

static void mouse_move(float x, float y, void *user_data) {

}

static void mouse_button_press(float x, float y, int button, void *user_data) {

}

static void mouse_button_release(float x, float y, int button, void *user_data) {

}

static void scroll(float dx, float dy, void *user_data) {

}

static void character(uint32_t c, void *user_data) {

}

static void paste(const char *text, void *user_data) {

}

static void initialize(void *user_data) {
	struct gral_demo *demo = user_data;
	struct gral_window_interface interface = {
		&close,
		&draw,
		&resize,
		&mouse_enter,
		&mouse_leave,
		&mouse_move,
		&mouse_button_press,
		&mouse_button_release,
		&scroll,
		&character,
		&paste
	};
	demo->window = gral_window_create(demo->application, 600, 400, "gral draw demo", &interface, demo);
}

int main(int argc, char **argv) {
	struct gral_demo demo;
	struct gral_application_interface interface = {&initialize};
	demo.application = gral_application_create("com.github.eyelash.libgral.demos.draw", &interface, &demo);
	int result = gral_application_run(demo.application, argc, argv);
	gral_window_delete(demo.window);
	gral_application_delete(demo.application);
	return result;
}
