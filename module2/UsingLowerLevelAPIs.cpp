#include <glib.h>

gboolean timer_callback(gpointer data)
{
    g_print("Custom GSource timer executed\n");

    return G_SOURCE_CONTINUE;
}

int main()
{
    GMainLoop* loop = g_main_loop_new(nullptr, FALSE);

    // Create timeout source
    GSource* source = g_timeout_source_new_seconds(2);

    // Register callback
    g_source_set_callback(
        source,
        timer_callback,
        nullptr,
        nullptr
    );

    // Attach source to default context
    guint source_id = g_source_attach(
        source,
        nullptr
    );

    g_print("Source ID: %u\n", source_id);

    // Start event processing
    g_main_loop_run(loop);

    // Destroy and release source
    g_source_destroy(source);
    g_source_unref(source);

    g_main_loop_unref(loop);

    return 0;
}