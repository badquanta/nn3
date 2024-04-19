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

Nuscape::Nuscape(Private)
{
    printf("Nuscape instance created\n");
    builder = gtk_builder_new_from_resource("/org/nuscape/ui/nuscape.glade");
    settings = WEBKIT_SETTINGS(gtk_builder_get_object(builder, "WebKitSettings"));
}

Nuscape::~Nuscape()
{
    printf("Nuscape instance destroyed\n");
}

std::shared_ptr<Naviguesser> Nuscape::createNaviguesser(WebKitWebView *related)
{
    std::shared_ptr<Naviguesser> naviguesser = Naviguesser::create(related, nextId);
    naviguessers[nextId] = naviguesser;
    nextId++;
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
        if(naviguessers.empty())
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