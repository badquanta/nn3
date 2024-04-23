#pragma once
#include <memory>
#include <unordered_map>
#include <gtk/gtk.h>
#include <webkit2/webkit2.h>
// Forward declaration of Naviguesser
class Naviguesser;

/** Nuscape class represents the application as a whole; and is hence is intended to be used as a static singleton */
class Nuscape : public std::enable_shared_from_this<Nuscape>
{
private:
    struct Private{};
    Nuscape(const Nuscape &) = delete;
    std::unordered_map<guint32, std::shared_ptr<Naviguesser>> naviguessers;
    guint32 nextId{0};
    static std::weak_ptr<Nuscape> instance;
public:
    Nuscape(Private);
    ~Nuscape();
    std::shared_ptr<Naviguesser> createNaviguesser(WebKitWebView *related = nullptr);
    std::shared_ptr<Naviguesser> getNaviguesser(guint32 id);
    bool removeNaviguesser(guint32 id);
    static std::shared_ptr<Nuscape> getInstance();

    GtkBuilder* builder;
    WebKitSettings* settings;
    GtkApplication* app;
};