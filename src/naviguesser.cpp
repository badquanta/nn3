#include "naviguesser.hpp"
#include "nuscape.hpp"

std::vector<std::shared_ptr<Naviguesser>> naviguesser_instances;

void update_WebKit_Uri_cb(GtkWidget *widget, GdkEventButton *event, Naviguesser *ui)
{
    webkit_web_view_load_uri(WEBKIT_WEB_VIEW(ui->WebView), gtk_entry_get_text(GTK_ENTRY(ui->UrlBar)));
}

void show_about_dialog(GtkWidget *widget, Naviguesser *ui)
{
    printf("About Menu item selected\n");
}

void web_view_go_back(GtkButton *button, Naviguesser *ui)
{
    webkit_web_view_go_back(WEBKIT_WEB_VIEW(ui->WebView));
}

void web_view_go_forward(GtkButton *button, Naviguesser *ui)
{
    webkit_web_view_go_forward(WEBKIT_WEB_VIEW(ui->WebView));
}

void web_view_reload(GtkButton *button, Naviguesser *ui)
{
    webkit_web_view_reload(WEBKIT_WEB_VIEW(ui->WebView));
}

void web_view_go_home(GtkButton *button, Naviguesser *ui)
{
    webkit_web_view_load_uri(WEBKIT_WEB_VIEW(ui->WebView), "http://www.theoldnet.com");
}

void web_view_stop(GtkButton *button, Naviguesser *ui)
{
    webkit_web_view_stop_loading(WEBKIT_WEB_VIEW(ui->WebView));
}

void *update_webkit_progress(Naviguesser *ui)
{
    gdouble progress = webkit_web_view_get_estimated_load_progress(WEBKIT_WEB_VIEW(ui->WebView));
    gtk_progress_bar_set_fraction(ui->StatusProgress, progress);
    if (progress < 1.0)
    {
    }
    else
    {
        printf("Nav#%d Removing update timeout: %d\n", ui->id,  ui->updateTimeoutId);
        g_source_remove(ui->updateTimeoutId);
        ui->updateTimeoutId = 0;
        gtk_widget_hide(GTK_WIDGET(ui->StatusProgress));
    }
    return ui;
}

void on_web_view_load_changed(WebKitWebView *web_view, WebKitLoadEvent load_event, Naviguesser *ui)
{
    const gchar *uri = webkit_web_view_get_uri(web_view);
    const gchar *title = webkit_web_view_get_title(web_view);
    printf("Nav#%d ", ui->id);
    std::ostringstream status{""};
    switch (load_event)
    {
    case WEBKIT_LOAD_STARTED:
        if (!ui->updateTimeoutId)
        {
            gtk_widget_show(GTK_WIDGET(ui->StatusProgress));
            ui->updateTimeoutId = g_timeout_add(250, (GSourceFunc)update_webkit_progress, ui);
            printf("Added update timeout: %d ", ui->updateTimeoutId);
        }
        status << "Resolving " << uri;
        gtk_statusbar_remove_all(ui->StatusBar, 0);
        gtk_statusbar_push(ui->StatusBar, 0, status.str().c_str());
        printf("Load Started\n");
        gtk_entry_set_text(GTK_ENTRY(ui->UrlBar), uri);
        gtk_action_set_sensitive(ui->action.Stop, TRUE);
        gtk_action_set_sensitive(ui->action.Reload, FALSE);
        gtk_image_set_from_animation(ui->UrlBarLogo, ui->LogoAnimated);
        break;
    case WEBKIT_LOAD_REDIRECTED:
        status << "Redirected to " << uri;
        gtk_statusbar_push(ui->StatusBar, 0, status.str().c_str());
        printf("Load Redirected\n");
        gtk_entry_set_text(GTK_ENTRY(ui->UrlBar), uri);
        break;
    case WEBKIT_LOAD_COMMITTED:
        status << "Downloading from " << uri;
        gtk_statusbar_push(ui->StatusBar, 0, status.str().c_str());
        printf("Load Committed\n");
        gtk_entry_set_text(GTK_ENTRY(ui->UrlBar), uri);

        break;
    case WEBKIT_LOAD_FINISHED:
        status << "Loaded " << uri;
        gtk_statusbar_push(ui->StatusBar, 3, status.str().c_str());
        gtk_action_set_sensitive(ui->action.Reload, TRUE);
        gtk_action_set_sensitive(ui->action.Stop, FALSE);
        printf("Load Finished\n");
        gtk_action_set_sensitive(ui->action.Back, webkit_web_view_can_go_back(WEBKIT_WEB_VIEW(ui->WebView)));
        gtk_action_set_sensitive(ui->action.Forward, webkit_web_view_can_go_forward(WEBKIT_WEB_VIEW(ui->WebView)));
        // printf("URI: %s\n", uri);
        gtk_entry_set_text(GTK_ENTRY(ui->UrlBar), uri);
        gtk_image_set_from_pixbuf(ui->UrlBarLogo, ui->Logo);
        gtk_window_set_title(GTK_WINDOW(ui->window), title);
        break;
    default:
        printf("Unknown WebKit Load Event: %d\n", load_event);
    }
}

void on_urlbar_activate(GtkEntry *entry, Naviguesser *ui)
{
    webkit_web_view_load_uri(WEBKIT_WEB_VIEW(ui->WebView), gtk_entry_get_text(entry));
}

void on_mouse_target_changed(WebKitWebView *web_view, WebKitHitTestResult *hit_test_result, guint modifiers, Naviguesser *ui)
{
    printf("Nav#%d Mouse Target Changed to: ", ui->id);
    // std::ostringstream status{""};

    if (webkit_hit_test_result_context_is_link(hit_test_result))
    {
        printf("Link: %s\n", webkit_hit_test_result_get_link_uri(hit_test_result));
        gtk_statusbar_push(ui->StatusBar, 1, webkit_hit_test_result_get_link_uri(hit_test_result));
    }
    else if (webkit_hit_test_result_context_is_image(hit_test_result))
    {
        printf("Image: %s\n", webkit_hit_test_result_get_image_uri(hit_test_result));
    }
    else if (webkit_hit_test_result_context_is_media(hit_test_result))
    {
        printf("Media: %s\n", webkit_hit_test_result_get_media_uri(hit_test_result));
    }
    else if (webkit_hit_test_result_context_is_editable(hit_test_result))
    {
        printf("Editable\n");
    }
    else
    {
        printf("None\n");
        gtk_statusbar_remove_all(ui->StatusBar, 1);
    }
}

gboolean on_web_process_crashed(WebKitWebView *web_view, Naviguesser *ui)
{
    printf("Nav#%d Web Process Crashed\n", ui->id);
    return TRUE;
}

GtkWidget *on_webkit_view_create(WebKitWebView *web_view, Naviguesser *ui)
{
    printf("Nav#%d Web View Create\n", ui->id);
    auto win = Nuscape::getInstance()->createNaviguesser(web_view);
    //gtk_container_remove(GTK_CONTAINER(win->NavViewFrame), GTK_WIDGET(win->WebView));
    //g_object_unref(win->WebView);
    //win->WebView = (WebKitWebView *)webkit_web_view_new_with_related_view(WEBKIT_WEB_VIEW(web_view));
    //gtk_container_add(GTK_CONTAINER(win->NavViewFrame), GTK_WIDGET(win->WebView));
    // web_view_set_related_ui(web_view, win->WebView);
    gtk_widget_show_all(GTK_WIDGET(win->window));
    return GTK_WIDGET(win->WebView);
}

void on_webkit_ready_to_show(WebKitWebView *web_view, Naviguesser *ui)
{
    printf("Web View Ready to Show\n");
    gtk_widget_show_all(GTK_WIDGET(ui->window));
}

void* monitor_webkit_process(Naviguesser *ui)
{
    //printf("Monitoring WebKit Process\n");
    if(webkit_web_view_get_is_web_process_responsive(WEBKIT_WEB_VIEW(ui->WebView)))
    {
        //printf("Web Process is responsive\n");
    }
    else
    {
        const gchar *uri = webkit_web_view_get_uri(WEBKIT_WEB_VIEW(ui->WebView));       
        printf("Nav#%d Web Process is not responsive, terminating and reloading '%s'....\n",ui->id, uri);
         
        webkit_web_view_terminate_web_process(WEBKIT_WEB_VIEW(ui->WebView));
        webkit_web_view_load_uri(WEBKIT_WEB_VIEW(ui->WebView), uri);
        

    }
    return ui;
}

void on_window_closed(GtkWidget *widget, Naviguesser *ui)
{
    printf("Nav#%d Window Closed\n", ui->id);
    gtk_widget_hide(GTK_WIDGET(ui->window));
    Nuscape::getInstance()->removeNaviguesser(ui->id);
}

Naviguesser::Naviguesser(Private, WebKitWebView *related, guint32 id) : id{id}
{

    // Load glade UI
    builder = gtk_builder_new_from_resource("/org/nuscape/ui/naviguesser.glade");
    monitorProcessId = g_timeout_add(1000, (GSourceFunc)monitor_webkit_process, this);
    // Get the main window
    window = GTK_APPLICATION_WINDOW(gtk_builder_get_object(builder, "Naviguesser"));
    g_object_set(window, "application", Nuscape::getInstance()->app, NULL);
    g_signal_connect(window, "destroy", G_CALLBACK(on_window_closed), this);

    // Frame that contains the WebKitView
    NavViewFrame = (GtkFrame *)gtk_builder_get_object(builder, "NavViewFrame");
    // Webkit setup
    //WebView = (WebKitWebView *)gtk_builder_get_object(builder, "NavView");
    if(related)
    {
        WebView = WEBKIT_WEB_VIEW(webkit_web_view_new_with_related_view(related));
    }
    else
    {
        WebView = WEBKIT_WEB_VIEW(webkit_web_view_new_with_settings(Nuscape::getInstance()->settings));
    }
    gtk_container_add(GTK_CONTAINER(NavViewFrame), GTK_WIDGET(WebView));
    GdkRGBA grey{0.75, 0.75, 0.75, 1.0};
    webkit_web_view_set_background_color(WebView, &grey);
    // webkit_web_view_load_uri(WEBKIT_WEB_VIEW(WebView), "http://www.theoldnet.com");
    //  on load changed
    g_signal_connect(WebView, "load-changed", G_CALLBACK(on_web_view_load_changed), this);
    g_signal_connect(WebView, "mouse-target-changed", G_CALLBACK(on_mouse_target_changed), this);
    g_signal_connect(WebView, "web-process-crashed", G_CALLBACK(on_web_process_crashed), this);
    g_signal_connect(WebView, "create", G_CALLBACK(on_webkit_view_create), this);

    // Statusbar
    StatusBar = (GtkStatusbar *)gtk_builder_get_object(builder, "NaviguessStatus");
    StatusProgress = (GtkProgressBar *)gtk_builder_get_object(builder, "StatusProgress");
    action.Exit = (GtkAction *)gtk_builder_get_object(builder, "ActionExit");
    g_signal_connect(action.Exit, "activate", G_CALLBACK(gtk_main_quit), this);
    action.Back = (GtkAction *)gtk_builder_get_object(builder, "ActionBack");
    g_signal_connect(action.Back, "activate", G_CALLBACK(web_view_go_back), this);
    action.Forward = (GtkAction *)gtk_builder_get_object(builder, "ActionForward");
    g_signal_connect(action.Forward, "activate", G_CALLBACK(web_view_go_forward), this);
    action.Home = (GtkAction *)gtk_builder_get_object(builder, "ActionHome");
    g_signal_connect(action.Home, "activate", G_CALLBACK(web_view_go_home), this);
    action.Reload = (GtkAction *)gtk_builder_get_object(builder, "ActionReload");
    g_signal_connect(action.Reload, "activate", G_CALLBACK(web_view_reload), this);
    action.Stop = (GtkAction *)gtk_builder_get_object(builder, "ActionStop");
    g_signal_connect(action.Stop, "activate", G_CALLBACK(web_view_stop), this);
    UrlBar = (GtkEntry *)gtk_builder_get_object(builder, "NaviguessUrlBar");
    g_signal_connect(UrlBar, "activate", G_CALLBACK(on_urlbar_activate), this);

    // connect("toolbarBackButton", "clicked", G_CALLBACK(web_view_go_back));
    // connect("toolbarForwardButton", "clicked", G_CALLBACK(web_view_go_forward));
    // connect("toolbarReloadButton", "clicked", G_CALLBACK(web_view_reload));

    LogoAnimated = gdk_pixbuf_animation_new_from_resource("/org/nuscape/ui/icons/logo-animated2.gif", NULL);
    Logo = gdk_pixbuf_new_from_resource("/org/nuscape/ui/icons/logo-animated2.gif", NULL);
    UrlBarLogo = (GtkImage *)gtk_builder_get_object(builder, "UrlBarLogo");

    printf("Nav#%d instance created\n", id);
    // gtk_widget_show(GTK_WIDGET(window));
}

Naviguesser::~Naviguesser()
{
    printf("Nav#%d instance destroyed\n", id);
    g_object_unref(builder);
    if(updateTimeoutId)
    {
        g_source_remove(updateTimeoutId);
    }
    if(monitorProcessId)
    {
        g_source_remove(monitorProcessId);
    }
}

std::shared_ptr<Naviguesser> Naviguesser::create(WebKitWebView *related, guint32 id)
{
    auto ui = std::make_shared<Naviguesser>(Private(), related, id);
    //naviguesser_instances.push_back(ui);
    return ui;
}

void Naviguesser::load_uri(std::string uri)
{
    webkit_web_view_load_uri(WEBKIT_WEB_VIEW(WebView), uri.c_str());
}