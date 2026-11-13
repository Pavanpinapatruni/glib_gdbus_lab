#include <glib.h>

struct Context
{
    GMainLoop* loop;
    guint counter;
};

gboolean high_priority_callback(gpointer data)
{
    g_print("[HIGH] Priority -100\n");
    return G_SOURCE_REMOVE;
}

gboolean default_priority_callback(gpointer data)
{
    g_print("[DEFAULT] Priority 0\n");
    return G_SOURCE_REMOVE;
}

gboolean low_priority_callback(gpointer data)
{
    g_print("[LOW] Priority 300\n");
    return G_SOURCE_REMOVE;
}

gboolean stop_callback(gpointer data)
{
    GMainLoop* loop = static_cast<GMainLoop*>(data);
    g_main_loop_quit(loop);
    return G_SOURCE_REMOVE;
}

int main()
{
    GMainLoop* loop = g_main_loop_new(nullptr, FALSE);

    GSource* high = g_idle_source_new();
    g_source_set_priority(high, G_PRIORITY_HIGH);
    g_source_set_callback(high, high_priority_callback, nullptr, nullptr);
    g_source_attach(high, nullptr);
    g_source_unref(high);

    GSource* normal = g_idle_source_new();
    g_source_set_priority(normal, G_PRIORITY_DEFAULT);
    g_source_set_callback(normal, default_priority_callback, nullptr, nullptr);
    g_source_attach(normal, nullptr);
    g_source_unref(normal);

    GSource* low = g_idle_source_new();
    g_source_set_priority(low, G_PRIORITY_LOW);
    g_source_set_callback(low, low_priority_callback, nullptr, nullptr);
    g_source_attach(low, nullptr);
    g_source_unref(low);

    g_timeout_add_seconds(2, stop_callback, loop);

    g_main_loop_run(loop);
    g_main_loop_unref(loop);

    return 0;
}