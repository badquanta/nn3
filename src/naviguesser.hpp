#pragma once

#include <gtk/gtk.h>
#include <webkit2/webkit2.h>
#include <sstream>
#include <string>
#include <memory>
#include <vector>

class Naviguesser : public std::enable_shared_from_this<Naviguesser>
{
private:
    struct Private
    {
    };

public:
    const guint32 id;
    Naviguesser(Private, WebKitWebView *related, guint32 id);
    ~Naviguesser();

    static std::shared_ptr<Naviguesser> create(WebKitWebView *related, guint32 id);

    void load_uri(std::string);

    GtkBuilder *builder;
    struct
    {
        GtkAction *Exit, *Back, *Home, *Forward, *Reload, *Stop;
    } action;
    GtkEntry *UrlBar;
    GtkImage *UrlBarLogo;
    WebKitWebView *WebView;
    GtkFrame *NavViewFrame;
    GtkStatusbar *StatusBar;
    GtkProgressBar *StatusProgress;
    GdkPixbufAnimation *LogoAnimated;
    GdkPixbuf *Logo;
    GtkApplicationWindow *window;

    guint updateTimeoutId{0};
    guint monitorProcessId{0};
};
