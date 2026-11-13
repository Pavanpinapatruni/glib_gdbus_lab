#include <glib.h>

struct BluetoothContext
{
    GMainLoop* loop;
    guint counter;
};

gboolean bluetooth_callback(gpointer user_data)
{
    g_print("[Bluetooth] Checking connection\n");

    BluetoothContext* context = static_cast<BluetoothContext*>(user_data);
    context->counter++;

    if (context->counter >= 5)
    {
        g_print("[Bluetooth] Maximum checks reached, stopping timer\n");
        return G_SOURCE_REMOVE;
    }

    return G_SOURCE_CONTINUE;
}

gboolean audio_callback(gpointer user_data)
{
    g_print("[Audio] Checking audio status\n");

    return G_SOURCE_CONTINUE;
}

gboolean initialization_callback(gpointer data)
{
    g_print("[System] Deferred initialization executed\n");

    // Execute only once
    return G_SOURCE_REMOVE;
}

gboolean stop_loop_callback(gpointer user_data)
{
    g_print("[System] Stopping main loop\n");
    GMainLoop* loop = static_cast<GMainLoop*>(user_data);
    g_main_loop_quit(loop);
    return G_SOURCE_REMOVE;
}

int main()
{
    GMainLoop* loop = g_main_loop_new(nullptr, FALSE);

    // Timer 1: Every 2 seconds
    BluetoothContext bt_context = { loop, 0 };
    guint bluetooth_timer = g_timeout_add_seconds(2, bluetooth_callback, &bt_context);

    // Timer 2: Every 5 seconds
    guint audio_timer = g_timeout_add_seconds(5, audio_callback, nullptr);

    // Idle callback
    guint idle_source = g_idle_add(initialization_callback, nullptr);

    g_print("Bluetooth timer ID: %u\n", bluetooth_timer);
    g_print("Audio timer ID: %u\n", audio_timer);
    g_print("Idle source ID: %u\n", idle_source);

    g_timeout_add_seconds(20, stop_loop_callback, loop);

    g_main_loop_run(loop);

    // Cleanup
    g_source_remove(audio_timer);
    g_main_loop_unref(loop);
    return 0;
}