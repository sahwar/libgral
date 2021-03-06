/*

Copyright (c) 2016-2018 Elias Aebi

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#ifndef GRAL_H
#define GRAL_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

enum {
	GRAL_PRIMARY_MOUSE_BUTTON = 1,
	GRAL_MIDDLE_MOUSE_BUTTON = 2,
	GRAL_SECONDARY_MOUSE_BUTTON = 3
};
enum {
	GRAL_CURSOR_DEFAULT = 68,
	GRAL_CURSOR_TEXT = 152,
	GRAL_CURSOR_HORIZONTAL_ARROWS = 108,
	GRAL_CURSOR_VERTICAL_ARROWS = 116,
	GRAL_CURSOR_NONE = -2
};

struct gral_application;
struct gral_application_interface {
	void (*initialize)(void *user_data);
};
struct gral_text;
struct gral_draw_context;
struct gral_window;
struct gral_window_interface {
	int (*close)(void *user_data);
	void (*draw)(struct gral_draw_context *draw_context, void *user_data);
	void (*resize)(int width, int height, void *user_data);
	void (*mouse_enter)(void *user_data);
	void (*mouse_leave)(void *user_data);
	void (*mouse_move)(float x, float y, void *user_data);
	void (*mouse_button_press)(float x, float y, int button, void *user_data);
	void (*mouse_button_release)(float x, float y, int button, void *user_data);
	void (*scroll)(float dx, float dy, void *user_data);
	void (*text)(const char *s, void *user_data);
	void (*paste)(const char *text, void *user_data);
	int (*timer)(void *user_data);
};
struct gral_file;


/*================
    APPLICATION
 ================*/

struct gral_application *gral_application_create(const char *id, const struct gral_application_interface *interface, void *user_data);
void gral_application_delete(struct gral_application *application);
int gral_application_run(struct gral_application *application, int argc, char **argv);


/*============
    DRAWING
 ============*/

struct gral_text *gral_text_create(struct gral_window *window, const char *text, float size);
void gral_text_delete(struct gral_text *text);
void gral_text_set_bold(struct gral_text *text, int start_index, int end_index);
float gral_text_get_width(struct gral_text *text, struct gral_draw_context *draw_context);
float gral_text_index_to_x(struct gral_text *text, int index);
int gral_text_x_to_index(struct gral_text *text, float x);

void gral_font_get_metrics(struct gral_window *window, float size, float *ascent, float *descent);

void gral_draw_context_draw_text(struct gral_draw_context *draw_context, struct gral_text *text, float x, float y, float red, float green, float blue, float alpha);
void gral_draw_context_close_path(struct gral_draw_context *draw_context);
void gral_draw_context_move_to(struct gral_draw_context *draw_context, float x, float y);
void gral_draw_context_line_to(struct gral_draw_context *draw_context, float x, float y);
void gral_draw_context_curve_to(struct gral_draw_context *draw_context, float x1, float y1, float x2, float y2, float x, float y);
void gral_draw_context_fill(struct gral_draw_context *draw_context, float red, float green, float blue, float alpha);
void gral_draw_context_fill_linear_gradient(struct gral_draw_context *draw_context, float start_x, float start_y, float end_x, float end_y, float start_red, float start_green, float start_blue, float start_alpha, float end_red, float end_green, float end_blue, float end_alpha);
void gral_draw_context_stroke(struct gral_draw_context *draw_context, float line_width, float red, float green, float blue, float alpha);
void gral_draw_context_draw_clipped(struct gral_draw_context *draw_context, void (*callback)(struct gral_draw_context *draw_context, void *user_data), void *user_data);
void gral_draw_context_draw_transformed(struct gral_draw_context *draw_context, float a, float b, float c, float d, float e, float f, void (*callback)(struct gral_draw_context *draw_context, void *user_data), void *user_data);


/*===========
    WINDOW
 ===========*/

struct gral_window *gral_window_create(struct gral_application *application, int width, int height, const char *title, const struct gral_window_interface *interface, void *user_data);
void gral_window_delete(struct gral_window *window);
void gral_window_request_redraw(struct gral_window *window, int x, int y, int width, int height);
void gral_window_set_minimum_size(struct gral_window *window, int minimum_width, int minimum_height);
void gral_window_set_cursor(struct gral_window *window, int cursor);
void gral_window_warp_cursor(struct gral_window *window, float x, float y);
void gral_window_show_open_file_dialog(struct gral_window *window, void (*callback)(const char *file, void *user_data), void *user_data);
void gral_window_show_save_file_dialog(struct gral_window *window, void (*callback)(const char *file, void *user_data), void *user_data);
void gral_window_clipboard_copy(struct gral_window *window, const char *text);
void gral_window_clipboard_request_paste(struct gral_window *window);
void gral_window_set_timer(struct gral_window *window, int milliseconds);


/*=========
    FILE
 =========*/

struct gral_file *gral_file_open_read(const char *path);
struct gral_file *gral_file_open_write(const char *path);
struct gral_file *gral_file_get_stdin(void);
struct gral_file *gral_file_get_stdout(void);
struct gral_file *gral_file_get_stderr(void);
void gral_file_close(struct gral_file *file);
size_t gral_file_read(struct gral_file *file, void *buffer, size_t size);
void gral_file_write(struct gral_file *file, const void *buffer, size_t size);
size_t gral_file_get_size(struct gral_file *file);


/*==========
    AUDIO
 ==========*/

void gral_audio_play(int (*callback)(int16_t *buffer, int frames, void *user_data), void *user_data);

#ifdef __cplusplus
}
#endif

#endif
