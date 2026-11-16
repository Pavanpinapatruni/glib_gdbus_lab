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
            G_TYPE_NONE,
            0
        );

    object_class->finalize = bluetooth_device_finalize;
    g_print("[Class] Device class initialized\n");
}

static void bluetooth_device_connect(BluetoothDevice* self)
{
    self->connected = TRUE;
    g_print("[Bluetooth] %s connected\n", self->device_name);

    g_signal_emit(
        self,
        bluetooth_device_signals[SIGNAL_DEVICE_CONNECTED],
        0
    );
}

static void bluetooth_device_disconnect(BluetoothDevice* self)
{
    self->connected = FALSE;
    g_print("[Bluetooth] %s disconnected\n", self->device_name);
}

static void on_device_connected(BluetoothDevice* self, gpointer user_data)
{
    g_print("[Handler] Received device-connected signal\n");
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
    g_signal_connect(
        device,
        "device-connected",
        G_CALLBACK(on_device_connected),
        nullptr
    );

    bluetooth_device_connect(device);
    bluetooth_device_disconnect(device);

    g_object_unref(device);
    return 0;
}