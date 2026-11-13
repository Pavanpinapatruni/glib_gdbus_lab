#include <glib.h>

struct BluetoothContext
{
    GMainLoop* loop;
    guint counter;
};

gboolean timeout_callback(gpointer user_data)
{
    BluetoothContext* context = static_cast<BluetoothContext*>(user_data);

    context->counter++;
    g_print("[Timer] Bluetooth status check: %u\n", context->counter);

    if (context->counter >=3)
    {
        g_print("[Timer] Maximum checks reached\n");

        // Stop the main loop
        g_main_loop_quit(context->loop);

        return G_SOURCE_REMOVE;
    }

    return G_SOURCE_CONTINUE;
}

int main()
{
    BluetoothContext context{};

    context.loop = g_main_loop_new(nullptr, FALSE);
    context.counter = 0;

    g_print("Application started\n");

    guint timer_id = g_timeout_add_seconds(
        2,
        timeout_callback,
        &context
    );

    g_print("Timer ID: %u\n", timer_id);

    g_main_loop_run(context.loop);

    g_print("Main loop stopped \n");

    g_main_loop_unref(context.loop);

    g_print("Application terminated\n");

    // Just about logging
    g_debug("Debug message");
    g_message("Application message");
    g_warning("Warning message");
    g_critical("Critical message");

    return 0;
}