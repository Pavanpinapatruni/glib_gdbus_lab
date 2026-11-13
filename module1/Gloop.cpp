#include <glib.h>

gboolean timeout_callback(gpointer user_data)
{
    g_print("Bluetooth status check\n");

    return G_SOURCE_CONTINUE;
}

int main()
{
    GMainLoop* loop = g_main_loop_new(nullptr, FALSE);

    g_timeout_add_seconds(
        5,
        timeout_callback,
        nullptr
    );

    g_main_loop_run(loop);

    g_main_loop_unref(loop);

    return 0;
}