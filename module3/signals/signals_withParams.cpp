#include <glib-object.h>

// Instance structure
typedef struct _BluetoothDevice
{
    GObject parent_instance;

    gchar* device_name;
    gboolean connected;

} BluetoothDevice;

// Class structure
typedef struct _BluetoothDeviceClass
{
    GObjectClass parent_class;
} BluetoothDeviceClass;

enum
{
    SIGNAL_DEVICE_CONNECTED,
    SIGNAL_DEVICE_DISCONNECTED,
    N_SIGNALS
};

static guint bluetooth_device_signals[N_SIGNALS];

// Register GObject type
G_DEFINE_TYPE(BluetoothDevice, bluetooth_device, G_TYPE_OBJECT)


// Instance initialization -- called when a new instance is created
static void bluetooth_device_init(BluetoothDevice* self)
{
    self->device_name = g_strdup("Unknown");
    self->connected = FALSE;

    g_print("[Instance] Device initialized\n");
}

static void bluetooth_device_finalize(GObject* object)
{
    BluetoothDevice* self = reinterpret_cast<BluetoothDevice*>(object);

    g_print("[Finalize] Cleaning up %s\n", self->device_name);

    g_clear_pointer(&self->device_name, g_free);
    G_OBJECT_CLASS(bluetooth_device_parent_class)->finalize(object);
}

// Class initialization -- called when the class is first registered with the type system
static void bluetooth_device_class_init(BluetoothDeviceClass* klass)
{
    GObjectClass* object_class = G_OBJECT_CLASS(klass);

    bluetooth_device_signals[SIGNAL_DEVICE_CONNECTED] =
        g_signal_new(
            "device-connected",
            G_TYPE_FROM_CLASS(klass),
            G_SIGNAL_RUN_LAST,
            0,
            nullptr,
            nullptr,
            nullptr,
            G_TYPE_NONE,    // Return type of the signal
            2,              // Number of parameters
            G_TYPE_STRING,  // Device name
            G_TYPE_STRING   // Bluetooth MAC address
        );

    bluetooth_device_signals[SIGNAL_DEVICE_DISCONNECTED] =
        g_signal_new(
            "device-disconnected",
            G_TYPE_FROM_CLASS(klass),
            G_SIGNAL_RUN_LAST,
            0,
            nullptr,
            nullptr,
            nullptr,
            G_TYPE_NONE,
            2,
            G_TYPE_STRING,
            G_TYPE_STRING
        );

    object_class->finalize = bluetooth_device_finalize;
    g_print("[Class] Device class initialized\n");
}

static void bluetooth_device_connect(BluetoothDevice* self)
{
    self->connected = TRUE;
    g_print("[Bluetooth] %s connected\n", self->device_name);

    const gchar* mac = "AA:BB:CC:DD:EE:FF";

    g_signal_emit(
        self,
        bluetooth_device_signals[SIGNAL_DEVICE_CONNECTED],
        0,
        self->device_name,
        mac
    );
}

static void bluetooth_device_disconnect(BluetoothDevice* self)
{
    self->connected = FALSE;
    g_print("[Bluetooth] %s disconnected\n", self->device_name);

    const gchar* mac = "AA:BB:CC:DD:EE:FF";

    g_signal_emit(
        self,
        bluetooth_device_signals[SIGNAL_DEVICE_DISCONNECTED],
        0,
        self->device_name,
        mac
    );
}

static void on_device_connected(BluetoothDevice* self, 
                                    const gchar* device_name, 
                                    const gchar* mac, 
                                    gpointer user_data)
{
    g_print("[Handler] Received device-connected signal\n");
    g_print("[Handler] Device name: %s\n", device_name);
    g_print("[Handler] MAC address: %s\n", mac);
}

static void on_device_disconnected(BluetoothDevice* self, 
                                    const gchar* device_name, 
                                    const gchar* mac, 
                                    gpointer user_data)
{
    g_print("[Handler] Received device-disconnected signal\n");
    g_print("[Handler] Device name: %s\n", device_name);
    g_print("[Handler] MAC address: %s\n", mac);
}

int main()
{
    // Create GObject instance
    BluetoothDevice* device = static_cast<BluetoothDevice*>(
        g_object_new(bluetooth_device_get_type(),nullptr)
    );

    // Change device name
    g_free(device->device_name);
    device->device_name = g_strdup("Pixel");

    // Invoke methods
    gulong handler_id = g_signal_connect(
        device,
        "device-connected",
        G_CALLBACK(on_device_connected),
        nullptr
    );

    g_signal_connect(
        device,
        "device-disconnected",
        G_CALLBACK(on_device_disconnected),
        nullptr
    );

    bluetooth_device_connect(device);
    bluetooth_device_disconnect(device);

    g_signal_handler_disconnect(device, handler_id);

    bluetooth_device_connect(device);
    bluetooth_device_disconnect(device);

    g_object_unref(device);
    return 0;
}