#include "naviguesser.hpp"
#include "nuscape.hpp"
/**
 * 
// GObject *ui.WebView;
GtkBuilder *builder;
// Just a bunch of pointers to the UI elements
struct
{
  struct
  {
    GtkAction *Exit, *Back, *Home, *Forward, *Reload, *Stop;
  } action;
  GtkEntry *UrlBar;
  GtkImage *UrlBarLogo;
  WebKitWebView *WebView;
  GtkStatusbar *StatusBar;
  GtkProgressBar *StatusProgress;
  GdkPixbufAnimation *LogoAnimated;
  GdkPixbuf *Logo;
} ui;

void update_WebKit_Uri_cb(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
  webkit_web_view_load_uri(WEBKIT_WEB_VIEW(ui.WebView), gtk_entry_get_text(GTK_ENTRY(ui.UrlBar)));
}

void show_about_dialog(GtkWidget *widget, gpointer data)
{
  printf("About Menu item selected\n");
}

void web_view_go_back(GtkButton *button, gpointer user_data)
{
  webkit_web_view_go_back(WEBKIT_WEB_VIEW(ui.WebView));
}

void web_view_go_forward(GtkButton *button, gpointer user_data)
{
  webkit_web_view_go_forward(WEBKIT_WEB_VIEW(ui.WebView));
}

void web_view_reload(GtkButton *button, gpointer user_data)
{
  webkit_web_view_reload(WEBKIT_WEB_VIEW(ui.WebView));
}

void web_view_go_home(GtkButton *button, gpointer user_data)
{
  webkit_web_view_load_uri(WEBKIT_WEB_VIEW(ui.WebView), "http://www.theoldnet.com");
}

void web_view_stop(GtkButton *button, gpointer user_data)
{
  webkit_web_view_stop_loading(WEBKIT_WEB_VIEW(ui.WebView));
}

void *update_webkit_progress(gpointer user_data)
{
  gdouble progress = webkit_web_view_get_estimated_load_progress(WEBKIT_WEB_VIEW(ui.WebView));
  gtk_progress_bar_set_fraction(ui.StatusProgress, progress);
  if (progress < 1.0)
  {
    g_timeout_add(100, (GSourceFunc)update_webkit_progress, NULL);
  }
  else
  {
    gtk_widget_hide(GTK_WIDGET(ui.StatusProgress));
  }
  return user_data;
}

void on_web_view_load_changed(WebKitWebView *web_view, WebKitLoadEvent load_event, gpointer user_data)
{
  const gchar *uri = webkit_web_view_get_uri(web_view);
  std::ostringstream status{""};
  switch (load_event)
  {
  case WEBKIT_LOAD_STARTED:
    gtk_widget_show(GTK_WIDGET(ui.StatusProgress));
    g_timeout_add_once(100, (GSourceOnceFunc)update_webkit_progress, NULL);
    status << "Resolving " << uri;
    gtk_statusbar_remove_all(ui.StatusBar, 0);
    gtk_statusbar_push(ui.StatusBar, 0, status.str().c_str());
    printf("Load Started\n");
    gtk_entry_set_text(GTK_ENTRY(ui.UrlBar), uri);
    gtk_action_set_sensitive(ui.action.Stop, TRUE);
    gtk_action_set_sensitive(ui.action.Reload, FALSE);
    gtk_image_set_from_animation(ui.UrlBarLogo, ui.LogoAnimated);
    break;
  case WEBKIT_LOAD_REDIRECTED:
    status << "Redirected to " << uri;
    gtk_statusbar_push(ui.StatusBar, 0, status.str().c_str());
    printf("Load Redirected\n");
    gtk_entry_set_text(GTK_ENTRY(ui.UrlBar), uri);
    break;
  case WEBKIT_LOAD_COMMITTED:
    status << "Downloading from " << uri;
    gtk_statusbar_push(ui.StatusBar, 0, status.str().c_str());
    printf("Load Committed\n");
    gtk_entry_set_text(GTK_ENTRY(ui.UrlBar), uri);

    break;
  case WEBKIT_LOAD_FINISHED:
    status << "Loaded " << uri;
    gtk_statusbar_push(ui.StatusBar, 3, status.str().c_str());
    gtk_action_set_sensitive(ui.action.Reload, TRUE);
    gtk_action_set_sensitive(ui.action.Stop, FALSE);
    printf("Load Finished\n");
    gtk_action_set_sensitive(ui.action.Back, webkit_web_view_can_go_back(WEBKIT_WEB_VIEW(ui.WebView)));
    gtk_action_set_sensitive(ui.action.Forward, webkit_web_view_can_go_forward(WEBKIT_WEB_VIEW(ui.WebView)));
    // printf("URI: %s\n", uri);
    gtk_entry_set_text(GTK_ENTRY(ui.UrlBar), uri);
    gtk_image_set_from_pixbuf(ui.UrlBarLogo, ui.Logo);
    break;
  default:
    printf("Unknown WebKit Load Event: %d\n", load_event);
  }
}

void on_urlbar_activate(GtkEntry *entry, gpointer user_data)
{
  webkit_web_view_load_uri(WEBKIT_WEB_VIEW(ui.WebView), gtk_entry_get_text(entry));
}

void on_mouse_target_changed(WebKitWebView *web_view, WebKitHitTestResult *hit_test_result, guint modifiers, gpointer user_data)
{
  printf("Mouse Target Changed to: ");
  // std::ostringstream status{""};

  if (webkit_hit_test_result_context_is_link(hit_test_result))
  {
    printf("Link: %s\n", webkit_hit_test_result_get_link_uri(hit_test_result));
    gtk_statusbar_push(ui.StatusBar, 1, webkit_hit_test_result_get_link_uri(hit_test_result));
    
  } else if(webkit_hit_test_result_context_is_image(hit_test_result)){
    printf("Image: %s\n", webkit_hit_test_result_get_image_uri(hit_test_result));
  } else if(webkit_hit_test_result_context_is_media(hit_test_result)){
    printf("Media: %s\n", webkit_hit_test_result_get_media_uri(hit_test_result));
  } else if(webkit_hit_test_result_context_is_editable(hit_test_result)){
    printf("Editable\n");
  }
  else
  {
    printf("None\n");
    gtk_statusbar_remove_all(ui.StatusBar, 1);
  }
}

gboolean on_web_process_crashed(WebKitWebView *web_view, gpointer user_data)
{
  printf("Web Process Crashed\n");
  return TRUE;
}

GtkWidget* on_webkit_view_create(WebKitWebView *web_view,  gpointer user_data)
{
  printf("Web View Create\n");
  return NULL;
}
*/
int main(int argc, char **argv)
{
  // GTK initialization
  gtk_init(&argc, &argv);
  
  auto nuscape = Nuscape::getInstance();
  //auto naviguesser = std::make_unique<Naviguesser>();
  // Setup the WebKit context.
  auto naviguesser = nuscape->createNaviguesser();
  naviguesser->load_uri("http://www.theoldnet.com");
  gtk_widget_show_all(GTK_WIDGET(naviguesser->window));
  
  gtk_main();

  return 0;
}
/**
int main(int argc, char *argv[])
{
  GtkWidget *win{nullptr}, *urlbar_goBtn{nullptr}, *vbox{nullptr}, *urlbar_hBox{nullptr};

  g_log_set_handler("Gtk", G_LOG_LEVEL_WARNING, (GLogFunc)gtk_false, NULL);
  gtk_init(&argc, &argv);
  g_log_set_handler("Gtk", G_LOG_LEVEL_WARNING, (GLogFunc)g_log_default_handler, NULL);

  win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(win), "Nuscape Naviguesser");
  gtk_window_set_position(GTK_WINDOW(win), GTK_WIN_POS_CENTER);

  g_signal_connect(win, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  // Container the menu bar, toolbar, url entry, webview, and status bar
  vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
  // Container for the menu bar
  GtkWidget *menubar = gtk_menu_bar_new();

  // Build File menu
  GtkWidget *fileMenu = gtk_menu_new();
  GtkWidget *fileMi = gtk_menu_item_new_with_label("File");
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(fileMi), fileMenu);
  //
  GtkWidget *fileNewWebBrowserMi = gtk_menu_item_new_with_mnemonic("New _Web Browser");
  gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), fileNewWebBrowserMi);
  // New Document
  GtkWidget *fileNewDocumentMi = gtk_menu_item_new_with_mnemonic("New _Document");
  gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), fileNewDocumentMi);
  GtkWidget *fileNewDocumentMenu = gtk_menu_new();
  // New Document -> HTML Document
  GtkWidget *fileNewDocumentHtmlMi = gtk_menu_item_new_with_mnemonic("HTML Document");
  gtk_menu_shell_append(GTK_MENU_SHELL(fileNewDocumentMenu), fileNewDocumentHtmlMi);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(fileNewDocumentMi), fileNewDocumentMenu);
  // Add Separator to File Menu
  gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), gtk_separator_menu_item_new());
  // Edit Document
  GtkWidget *fileEditDocumentMi = gtk_menu_item_new_with_mnemonic("_Edit Document");
  gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), fileEditDocumentMi);
  // Add Separator to File Menu
  gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), gtk_separator_menu_item_new());
  // New Mail Document
  GtkWidget *fileNewMailDocumentMi = gtk_menu_item_new_with_mnemonic("New _Mail Document");
  gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), fileNewMailDocumentMi);
  // Mail Document...
  GtkWidget *fileMailDocumentMi = gtk_menu_item_new_with_mnemonic("_Mail Document...");
  gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), fileMailDocumentMi);
  // Add Separator to File Menu
  gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), gtk_separator_menu_item_new());
  // Open Location...
  GtkWidget *fileOpenLocationMi = gtk_menu_item_new_with_mnemonic("_Open Location...");
  gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), fileOpenLocationMi);
  // Open File in Browser
  GtkWidget *fileOpenFileInBrowserMi = gtk_menu_item_new_with_mnemonic("Open _File in Browser");
  gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), fileOpenFileInBrowserMi);
  // Open File in Editor
  GtkWidget *fileOpenFileInEditorMi = gtk_menu_item_new_with_mnemonic("Open File in Ed_itor");
  gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), fileOpenFileInEditorMi);
  // Add Quit to File Menu
  GtkWidget *fileExitMi = gtk_menu_item_new_with_mnemonic("E_xit");
  gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu), fileExitMi);
  g_signal_connect(G_OBJECT(fileExitMi), "activate", G_CALLBACK(gtk_main_quit), NULL);

  // Add File Menu to Menu Bar
  gtk_menu_shell_append(GTK_MENU_SHELL(menubar), fileMi);

  // Build Help menu
  GtkWidget *helpMenu = gtk_menu_new();
  GtkWidget *helpMi = gtk_menu_item_new_with_mnemonic("Help");
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(helpMi), helpMenu);
  // Add About to Help Menu
  GtkWidget *helpAboutMi = gtk_menu_item_new_with_mnemonic("About");
  gtk_menu_shell_append(GTK_MENU_SHELL(helpMenu), helpAboutMi);
  g_signal_connect(G_OBJECT(helpAboutMi), "activate", G_CALLBACK(show_about_dialog), NULL);

  // Add Help Menu to Menu Bar
  gtk_menu_shell_append(GTK_MENU_SHELL(menubar), helpMi);

  // Add the menu bar to the vbox
  gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);



  // Container for the url entry and go button
  urlbar_hBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);

  urlbar_entry = gtk_entry_new();
  gtk_entry_set_text(GTK_ENTRY(urlbar_entry), "http://www.theoldnet.com");
  // Add the entry to the url_hbox
  gtk_box_pack_start(GTK_BOX(urlbar_hBox), urlbar_entry, TRUE, TRUE, 0);

  urlbar_goBtn = gtk_button_new_with_label("Go");
  g_signal_connect(urlbar_goBtn, "button-press-event", G_CALLBACK(update_WebKit_Uri_cb), (gpointer)win);
  // Add the button to the url_hbox
  gtk_box_pack_start(GTK_BOX(urlbar_hBox), urlbar_goBtn, FALSE, FALSE, 0);
  // Add the url_hbox to the vbox
  gtk_box_pack_start(GTK_BOX(vbox), urlbar_hBox, FALSE, FALSE, 0);
  //
  ui.WebView = WEBKIT_WEB_VIEW(webkit_web_view_new());
  // Navigate to the url in the url entry (TODO make this a home page setting)
  webkit_web_view_load_uri(ui.WebView, gtk_entry_get_text(GTK_ENTRY(urlbar_entry)));
  // Add the webview to the vbox
  gtk_box_pack_start(GTK_BOX(vbox), GTK_WIDGET(ui.WebView), TRUE, TRUE, 0);
  // Add the vbox to the window
  gtk_container_add(GTK_CONTAINER(win), vbox);
  //
  gtk_window_set_default_size(GTK_WINDOW(win), 800, 600);
  gtk_widget_show_all(win);
  gtk_main();
  return 0;
}
**/