#include "nuscape.hpp"
#include "naviguesser.hpp"

std::weak_ptr<Nuscape> Nuscape::instance;

std::shared_ptr<Nuscape> Nuscape::getInstance()
{
    std::shared_ptr<Nuscape> app;
    if (instance.expired())
    {
        app = std::make_shared<Nuscape>(Private());
        instance = app;
    }
    else
    {
        app = instance.lock();
    }

    return app;
}

void on_app_new_browser_window(GSimpleAction *simple, GVariant *p, gpointer user_data)
{
    printf("On App New Browser Window\n");
}

void on_app_startup(GApplication *app, Nuscape *nuscape)
{
    printf("On App Startup\n");
}

void on_app_activate(GApplication *app, Nuscape *nuscape)
{
    printf("On App Activate\n"); // auto naviguesser = std::make_unique<Naviguesser>();
    // Setup the WebKit context.
    auto naviguesser = nuscape->createNaviguesser();
    naviguesser->load_uri("http://www.theoldnet.com");
    gtk_widget_show_all(GTK_WIDGET(naviguesser->window));

    // gtk_main();
}

Nuscape::Nuscape(Private)
{
    printf("Nuscape instance created\n");
    app = GTK_APPLICATION(gtk_application_new("org.badquanta.nuscape", G_APPLICATION_DEFAULT_FLAGS));
    g_signal_connect(app, "startup", G_CALLBACK(on_app_startup), this);
    g_signal_connect(app, "activate", G_CALLBACK(on_app_activate), this);
    builder = gtk_builder_new_from_resource("/org/nuscape/ui/nuscape.glade");
    settings = WEBKIT_SETTINGS(gtk_builder_get_object(builder, "WebKitSettings"));
    const GActionEntry entries[] = {
        {"new-browser-window", on_app_new_browser_window},
    };
    g_action_map_add_action_entries(G_ACTION_MAP(app), entries, G_N_ELEMENTS(entries), NULL);
}

Nuscape::~Nuscape()
{
    printf("Nuscape instance destroyed\n");
    g_object_unref(app);
    g_object_unref(builder);
}

std::shared_ptr<Naviguesser> Nuscape::createNaviguesser(WebKitWebView *related)
{
    std::shared_ptr<Naviguesser> naviguesser = Naviguesser::create(related, nextId);
    naviguessers[nextId] = naviguesser;
    nextId++;
    gtk_application_add_window(app, GTK_WINDOW(naviguesser->window));
    return naviguesser;
}

std::shared_ptr<Naviguesser> Nuscape::getNaviguesser(guint32 id)
{
    if (naviguessers.contains(id))
    {
        return naviguessers.at(id);
    }
    else
    {
        return nullptr;
    }
}

bool Nuscape::removeNaviguesser(guint32 id)
{
    if (naviguessers.contains(id))
    {
        naviguessers.erase(id);
        if (naviguessers.empty())
        {
            printf("All naviguessers removed; quitting\n");
            gtk_main_quit();
        }

        return true;
    }
    else
    {
        return false;
    }
}