#include <gral.h>
#include <stdio.h>

struct demo {
	struct gral_application *application;
	struct gral_window *window;
};

static int close(void *user_data) {
	return 1;
}

static void draw(struct gral_draw_context *draw_context, void *user_data) {

}

static void resize(int width, int height, void *user_data) {
	printf("resize: %dx%d\n", width, height);
}

static void mouse_enter(void *user_data) {
	printf("mouse enter\n");
}

static void mouse_leave(void *user_data) {
	printf("mouse leave\n");
}

static void mouse_move(float x, float y, void *user_data) {
	printf("mouse move: {%f, %f}\n", x, y);
}

static void mouse_button_press(float x, float y, int button, void *user_data) {
	printf("mouse button press: {%f, %f}\n", x, y);
}

static void mouse_button_release(float x, float y, int button, void *user_data) {
	printf("mouse button release: {%f, %f}\n", x, y);
}

static void scroll(float dx, float dy, void *user_data) {
	printf("scroll {%f, %f}\n", dx, dy);
}

static uint32_t utf8_get_next(const char **utf8) {
	const char *c = *utf8;
	if ((c[0] & 0x80) == 0x00) {
		*utf8 += 1;
		return c[0];
	}
	if ((c[0] & 0xE0) == 0xC0) {
		*utf8 += 2;
		return (c[0] & 0x1F) << 6 | (c[1] & 0x3F);
	}
	if ((c[0] & 0xF0) == 0xE0) {
		*utf8 += 3;
		return (c[0] & 0x0F) << 12 | (c[1] & 0x3F) << 6 | (c[2] & 0x3F);
	}
	if ((c[0] & 0xF8) == 0xF0) {
		*utf8 += 4;
		return (c[0] & 0x07) << 18 | (c[1] & 0x3F) << 12 | (c[2] & 0x3F) << 6 | (c[3] & 0x3F);
	}
	fprintf(stderr, "invalid UTF-8\n");
	return 0;
}

static void text(const char *s, void *user_data) {
	while (*s) {
		printf("text: U+%X\n", utf8_get_next(&s));
	}
}

static void paste(const char *text, void *user_data) {

}

static int timer(void *user_data) {
	printf("timer\n");
	return 1;
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
	demo->window = gral_window_create(demo->application, 600, 400, "gral events demo", &interface, demo);
	gral_window_set_timer(demo->window, 1000);
}

int main(int argc, char **argv) {
	struct demo demo;
	struct gral_application_interface interface = {&initialize};
	demo.application = gral_application_create("com.github.eyelash.libgral.demos.events", &interface, &demo);
	int result = gral_application_run(demo.application, argc, argv);
	gral_window_delete(demo.window);
	gral_application_delete(demo.application);
	return result;
}
