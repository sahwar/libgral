#include <gral.h>

struct demo {
	struct gral_application *application;
	struct gral_window *window;
	struct gral_text *text;
	float cursor_x;
	float ascent;
	float descent;
};

static int close(void *user_data) {
	return 1;
}

static void add_rectangle(struct gral_draw_context *draw_context, float x, float y, float width, float height) {
	gral_draw_context_move_to(draw_context, x, y);
	gral_draw_context_line_to(draw_context, x + width, y);
	gral_draw_context_line_to(draw_context, x + width, y + height);
	gral_draw_context_line_to(draw_context, x, y + height);
	gral_draw_context_close_path(draw_context);
}

static void draw(struct gral_draw_context *draw_context, void *user_data) {
	struct demo *demo = user_data;
	float width = gral_text_get_width(demo->text, draw_context);
	float height = demo->ascent + demo->descent;
	add_rectangle(draw_context, 50.f, 50.f, width, 1.f);
	gral_draw_context_fill(draw_context, 1.f, 0.f, 0.f, 1.f);
	add_rectangle(draw_context, 50.f, 50.f - demo->ascent, width, height);
	gral_draw_context_fill(draw_context, 1.f, 0.f, 0.f, .2f);
	gral_draw_context_draw_text(draw_context, demo->text, 50.f, 50.f, 0.f, 0.f, 1.f, 1.f);
	add_rectangle(draw_context, 50.f + demo->cursor_x, 50.f - demo->ascent, 1.f, height);
	gral_draw_context_fill(draw_context, 1.f, 0.f, 0.f, 1.f);
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
	struct demo *demo = user_data;
	int index = gral_text_x_to_index(demo->text, x - 50.f);
	demo->cursor_x = gral_text_index_to_x(demo->text, index);
	gral_window_request_redraw(demo->window, 0, 0, 600, 400);
}

static void mouse_button_release(float x, float y, int button, void *user_data) {

}

static void scroll(float dx, float dy, void *user_data) {

}

static void text(const char *s, void *user_data) {

}

static void paste(const char *text, void *user_data) {

}

static int timer(void *user_data) {
	return 0;
}

static void initialize(void *user_data) {
	struct demo *demo = user_data;
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
		&text,
		&paste,
		&timer
	};
	demo->window = gral_window_create(demo->application, 600, 400, "gral text demo", &interface, demo);
	demo->text = gral_text_create(demo->window, "gral text demo", 16.f);
	gral_text_set_bold(demo->text, 5, 9);
	demo->cursor_x = 0.f;
	gral_font_get_metrics(demo->window, 16.f, &demo->ascent, &demo->descent);
}

int main(int argc, char **argv) {
	struct demo demo;
	struct gral_application_interface interface = {&initialize};
	demo.application = gral_application_create("com.github.eyelash.libgral.demos.text", &interface, &demo);
	int result = gral_application_run(demo.application, argc, argv);
	gral_text_delete(demo.text);
	gral_window_delete(demo.window);
	gral_application_delete(demo.application);
	return result;
}
