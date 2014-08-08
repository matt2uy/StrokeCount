#include <pebble.h>

static Window *window;
static TextLayer *text_layer;
int num_of_strokes[] = {0, 5, 5, 5, 5, 5, 5, 5, 5,    // there is no hole 0
                        5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5};
int current_hole = 1;
bool show_info = true;

/////// Stroke manipulation functions

static void add_stroke() {
  num_of_strokes[current_hole]++;
}
static void subtract_stroke() {
  num_of_strokes[current_hole]--;
  if(num_of_strokes[current_hole] < 0) {
    num_of_strokes[current_hole] = 0;    
  }
}

static void next_hole() {
  if (current_hole == 19) {
    current_hole = 1;
  }
  else {
    current_hole++;
  }
}
static void prev_hole() {
  if (current_hole == 0) {
    current_hole = 18;
  }
  else {
    current_hole--;
  }
}

/////// Click event functions

static void select_long_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (current_hole == 19) {
    current_hole = 18;
    static char body_text[50];
    snprintf(body_text, sizeof(body_text), "   Hole %u  Stroke: %u", current_hole, num_of_strokes[current_hole]);
    text_layer_set_text(text_layer, body_text);
  }
  //Window *window = (Window *)context; // This context defaults to the window, but may be changed with \ref window_set_click_context.
  else if (current_hole == 1) {
    current_hole = 18;
    static char body_text[50];
    snprintf(body_text, sizeof(body_text), "   Hole %u  Stroke: %u", current_hole, num_of_strokes[current_hole]);
    text_layer_set_text(text_layer, body_text);
  }
  else {
    current_hole--;
    static char body_text[50];
    snprintf(body_text, sizeof(body_text), "   Hole %u  Stroke: %u", current_hole, num_of_strokes[current_hole]);
    text_layer_set_text(text_layer, body_text);
    //show_info = true;
  }
}
static void select_long_click_release_handler(ClickRecognizerRef recognizer, void *context) {
  //... called when long click is released ...
  // do nothing
  Window *window = (Window *)context; // This context defaults to the window, but may be changed with \ref window_set_click_context.
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (current_hole == 18){
    current_hole++;
    int total_score = 0;
    for (int a=1; a<19; a++)
          {
    		total_score+=num_of_strokes[a];
          }
    
    static char body_text[50];
    snprintf(body_text, sizeof(body_text), "You shot: %u", total_score);
    text_layer_set_text(text_layer, body_text);
    show_info = true;
  }
  else if (current_hole == 19) {
    current_hole = 1;
    static char body_text[50];
    snprintf(body_text, sizeof(body_text), "   Hole %u  Stroke: %u", current_hole, num_of_strokes[current_hole]);
    text_layer_set_text(text_layer, body_text);
    show_info = true;
  }
  else {
    current_hole++;
    static char body_text[50];
    snprintf(body_text, sizeof(body_text), "   Hole %u  Stroke: %u", current_hole, num_of_strokes[current_hole]);
    text_layer_set_text(text_layer, body_text);
    show_info = true;
  }

  
  
  
  // show info
  /*
  if (show_info == true){
    text_layer_set_text(text_layer, "Press Up/Down for Strokes, Select for Next hole");
    show_info = false;
  }
  else {
    static char body_text[50];
  snprintf(body_text, sizeof(body_text), "Hole: %u | Stroke: %u", current_hole, num_of_strokes[current_hole]);
    text_layer_set_text(text_layer, body_text);
    show_info = true;
  }
  */
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  add_stroke();
  static char body_text[50];
  snprintf(body_text, sizeof(body_text), "   Hole %u  Stroke: %u", current_hole, num_of_strokes[current_hole]);
  text_layer_set_text(text_layer, body_text);
  show_info = true;
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  subtract_stroke();
    
  static char body_text[50];
  snprintf(body_text, sizeof(body_text), "   Hole %u  Stroke: %u", current_hole, num_of_strokes[current_hole]);
  text_layer_set_text(text_layer, body_text);
  show_info = true;
}

///////////

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
   // long click config:
  window_long_click_subscribe(BUTTON_ID_SELECT, 700, select_long_click_handler, select_long_click_release_handler);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  text_layer = text_layer_create(GRect(0, 0, 144, 154));
  text_layer_set_text(text_layer, "Press Up/Down for Strokes, Select for Next hole");
	text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
}

static void window_unload(Window *window) {
  text_layer_destroy(text_layer);
}

static void init(void) {
  window = window_create();
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
	.load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}