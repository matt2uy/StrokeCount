#include <pebble.h>

static Window *window;
static TextLayer *text_layer;
int num_of_strokes[30] = {0, 0, 0, 0, 0, 0, 0, 0, 0,    // there is no hole 0
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// duntroon
char course_name[] = "Duntroon";
int hole_yardage[30] = {0, 490, 330, 102, 319, 320, 346, 317, 156, 415,   // there is no hole 0
                      452, 311, 288, 158, 207, 502, 335, 406, 442};
int hole_par[30] = {0, 5, 4, 3, 4, 4, 4, 4, 3, 4,   // there is no hole 0
                      5, 4, 4, 3, 3, 5, 4, 4, 4};
int course_par = 0;
int num_of_putts[30] = {0, 0, 0, 0, 0, 0, 0, 0, 0,    // there is no hole 0
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
char num_of_FIR[30] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',    // there is no hole 0
                       ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
int num_of_possible_FIR = 0;
int current_hole = 0;
int holes_in_round = 18;

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
  if (current_hole > 18) {
    current_hole = 1;
  }
  else {
    current_hole++;
  }
}
static void prev_hole() {
  if (current_hole == 0) {
    current_hole = holes_in_round;
  }
  else {
    current_hole--;
  }
}
/////// update window
static void show_current_hole() {
  static char body_text[100];
  snprintf(body_text, sizeof(body_text), "Hole %u | Par %u\n%u Yards\n - - - - - \nStroke: %u\n FIR: %c | %u Putts", current_hole, hole_par[current_hole], hole_yardage[current_hole], num_of_strokes[current_hole], num_of_FIR[current_hole], num_of_putts[current_hole]);
  text_layer_set_text(text_layer, body_text);
}
static void add_and_show_total() {
  current_hole++;
  int total_score = 0;
  int total_putts = 0;
  int total_GIR = 0;
  int total_FIR = 0;
  int total_tee_left = 0;
  int total_tee_right = 0;
  int total_penalties = 0;
  // Fairway in regulation
  for (int a=1; a<holes_in_round+1; a++) {
    if (num_of_FIR[a] == 'f') { // if FIR is true
      total_FIR++;
    }
    else if (num_of_FIR[a] == 'l') {
      total_tee_left++;
    }
    else if (num_of_FIR[a] == 'r') {
      total_tee_right++;
    }
  }
  /*// Green in regulation
  for (int a=1; a<holes_in_round+1; a++) {
    if (num_of_strokes[a]-num_of_putts[a] <= hole_par[a]) { // if GIR is true
      num_of_GIR[a] = true;
    }
  }*/
  for (int a=1; a<holes_in_round+1; a++) {
    total_score+=num_of_strokes[a];
    total_putts+=num_of_putts[a];
    //total_GIR+=num_of_GIR[a];
    //total_penalties+=num_of_penalties[a];
  }
  
  static char body_text[100];
  snprintf(body_text, sizeof(body_text), "%u Strokes\n%u Putts\n\n%u < %u/%u FIR > %u\n", total_score, total_putts, total_tee_left, total_FIR, num_of_possible_FIR, total_tee_right);
  text_layer_set_text(text_layer, body_text);
}
/////// Click event functions
////////////////////////////////////////
/*static void up_long_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (num_of_FIR[current_hole] == ' ' || num_of_FIR[current_hole] == 'r') {
    num_of_FIR[current_hole] = 'f';
  }
  else if (num_of_FIR[current_hole] == 'f') {
    num_of_FIR[current_hole] = 'l';
  }
  else if (num_of_FIR[current_hole] == 'l') {
    num_of_FIR[current_hole] = 'r';
  }
  show_current_hole();
}
static void up_long_click_release_handler(ClickRecognizerRef recognizer, void *context) {
  //... called when long click is released ...
  // do nothing
  Window *window = (Window *)context; // This context defaults to the window, but may be changed with \ref window_set_click_context.
}
////////////////////////////////////////
static void down_long_click_handler(ClickRecognizerRef recognizer, void *context) {
  num_of_putts[current_hole]++;
  show_current_hole();
}
static void down_long_click_release_handler(ClickRecognizerRef recognizer, void *context) {
  //... called when long click is released ...
  // do nothing
  Window *window = (Window *)context; // This context defaults to the window, but may be changed with \ref window_set_click_context.
}*/
///////////////////////////////////////
static void select_long_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (current_hole > holes_in_round) {
    current_hole = holes_in_round;
  }
  else if (current_hole == 1) {
    current_hole = holes_in_round;
  }
  else {
    current_hole--;
  }
  show_current_hole();
}

static void select_long_click_release_handler(ClickRecognizerRef recognizer, void *context) {
  //... called when long click is released ...
  // do nothing
  Window *window = (Window *)context; // This context defaults to the window, but may be changed with \ref window_set_click_context.
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (current_hole == holes_in_round){
    add_and_show_total();
  }
  else if (current_hole > holes_in_round) {
    current_hole = 1;
    show_current_hole();
  }
  else {
    current_hole++;
    show_current_hole();
  }
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (current_hole == 0) {
    current_hole = 1;
  }
  add_stroke();
  show_current_hole();
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (current_hole == 0) {
    current_hole = 1;
  }
  subtract_stroke();
  show_current_hole();
}

///////////

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
   // long click
  window_long_click_subscribe(BUTTON_ID_SELECT, 700, select_long_click_handler, select_long_click_release_handler);
  //window_long_click_subscribe(BUTTON_ID_SELECT, 700, up_long_click_handler, up_long_click_release_handler);
  //window_long_click_subscribe(BUTTON_ID_SELECT, 700, down_long_click_handler, down_long_click_release_handler);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
  text_layer = text_layer_create(GRect(0, 0, 144, 154));
  text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text(text_layer, "More strokes >\n\nNext Hole  ->\n\nLess strokes >");
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
}

static void window_unload(Window *window) {
  text_layer_destroy(text_layer);
}

static void init(void) {
  for (int a=1; a < holes_in_round+1; a++) { //  add up course par
    // get number of FIR's possible
    if (hole_par[a] > 3) { // par 4's and 5's
      num_of_possible_FIR++;
    }  
  course_par+=hole_par[a];
  }
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