
#include <glib.h>
#include <iostream>

int main()
{
    // GLib data types
    gint retry_count = 3;
    gboolean bluetooth_connected = TRUE;

    // GLib string allocation
    gchar* device_name = g_strdup("Pixel");

    // Print information
    g_print("Device Name: %s\n", device_name);
    g_print("Retry Count: %d\n", retry_count);

    if (bluetooth_connected)
    {
        g_print("Bluetooth Connected\n");
    }

    // Release allocated memory
    g_free(device_name);
    device_name = nullptr;

    return 0;
}
