#include <stdlib.h>
#include <gtk/gtk.h>
#include <webkit2/webkit2.h>

GObject *webView;
GObject *urlbar_entry;
GtkBuilder *builder;
struct
{
  GObject *back, *forward, *reload, *home;
} toolbar;

void update_WebKit_Uri_cb(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
  webkit_web_view_load_uri(WEBKIT_WEB_VIEW(webView), gtk_entry_get_text(GTK_ENTRY(urlbar_entry)));
}

void show_about_dialog(GtkWidget *widget, gpointer data)
{
  printf("About Menu item selected\n");
}

void web_view_go_back(GtkButton *button, gpointer user_data)
{
  webkit_web_view_go_back(WEBKIT_WEB_VIEW(webView));
}

void web_view_go_forward(GtkButton *button, gpointer user_data)
{
  webkit_web_view_go_forward(WEBKIT_WEB_VIEW(webView));
}

void web_view_reload(GtkButton *button, gpointer user_data)
{
  webkit_web_view_reload(WEBKIT_WEB_VIEW(webView));
}

void web_view_go_home(GtkButton *button, gpointer user_data)
{
  webkit_web_view_load_uri(WEBKIT_WEB_VIEW(webView), "http://www.theoldnet.com");
}

void on_web_view_load_changed(WebKitWebView *web_view, WebKitLoadEvent load_event, gpointer user_data)
{
  const gchar *uri = webkit_web_view_get_uri(web_view);
  switch (load_event)
  {
  case WEBKIT_LOAD_STARTED:
    printf("Load Started\n");
    break;
  case WEBKIT_LOAD_REDIRECTED:
    printf("Load Redirected\n");
    break;
  case WEBKIT_LOAD_COMMITTED:
    printf("Load Committed\n");
    if (webkit_web_view_can_go_back(WEBKIT_WEB_VIEW(webView)))
      gtk_widget_set_sensitive(GTK_WIDGET(toolbar.back), TRUE);
    else
      gtk_widget_set_sensitive(GTK_WIDGET(toolbar.back), FALSE);
    if(webkit_web_view_can_go_forward(WEBKIT_WEB_VIEW(webView)))
      gtk_widget_set_sensitive(GTK_WIDGET(toolbar.forward), TRUE);
    else
      gtk_widget_set_sensitive(GTK_WIDGET(toolbar.forward), FALSE);

    break;
  case WEBKIT_LOAD_FINISHED:
    printf("Load Finished\n");

    printf("URI: %s\n", uri);
    //   gtk_entry_set_text(GTK_ENTRY(urlbar_entry), uri);
    break;
  default:
    printf("Unknown WebKit Load Event: %d\n", load_event);
  }
}

void connect(const char *widgetName, const char *signal, GCallback callback)
{
  printf("Connecting %s signal %s", widgetName, signal);
  GObject *widget = gtk_builder_get_object(builder, widgetName);
  g_signal_connect(widget, signal, callback, NULL);
}

int main(int argc, char **argv)
{
  // GTK initialization
  gtk_init(&argc, &argv);

  // Load glade UI
  builder = gtk_builder_new();
  gtk_builder_add_from_file(builder, "naviguesser.glade", NULL);

  // Get the main window
  GObject *window = gtk_builder_get_object(builder, "Naviguesser");
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  // Webkit setup
  webView = gtk_builder_get_object(builder, "NavView");
  webkit_web_view_load_uri(WEBKIT_WEB_VIEW(webView), "http://www.theoldnet.com");

  // on load changed
  g_signal_connect(webView, "load-changed", G_CALLBACK(on_web_view_load_changed), NULL);

  toolbar.back = gtk_builder_get_object(builder, "toolbarBackButton");
  toolbar.forward = gtk_builder_get_object(builder, "toolbarForwardButton");
  toolbar.reload = gtk_builder_get_object(builder, "toolbarReloadButton");
  toolbar.home = gtk_builder_get_object(builder, "toolbarHomeButton");
  g_signal_connect(toolbar.back, "clicked", G_CALLBACK(web_view_go_back), NULL);
  g_signal_connect(toolbar.forward, "clicked", G_CALLBACK(web_view_go_forward), NULL);
  g_signal_connect(toolbar.reload, "clicked", G_CALLBACK(web_view_reload), NULL);
  g_signal_connect(toolbar.home, "clicked", G_CALLBACK(web_view_go_home), NULL);
  // connect("toolbarBackButton", "clicked", G_CALLBACK(web_view_go_back));
  // connect("toolbarForwardButton", "clicked", G_CALLBACK(web_view_go_forward));
  // connect("toolbarReloadButton", "clicked", G_CALLBACK(web_view_reload));

  gtk_widget_show(GTK_WIDGET(window));

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
  webView = WEBKIT_WEB_VIEW(webkit_web_view_new());
  // Navigate to the url in the url entry (TODO make this a home page setting)
  webkit_web_view_load_uri(webView, gtk_entry_get_text(GTK_ENTRY(urlbar_entry)));
  // Add the webview to the vbox
  gtk_box_pack_start(GTK_BOX(vbox), GTK_WIDGET(webView), TRUE, TRUE, 0);
  // Add the vbox to the window
  gtk_container_add(GTK_CONTAINER(win), vbox);
  //
  gtk_window_set_default_size(GTK_WINDOW(win), 800, 600);
  gtk_widget_show_all(win);
  gtk_main();
  return 0;
}
**/