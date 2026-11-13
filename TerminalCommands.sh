# List available bus services
busctl --system list

# Inspect a remote object's interfaces
gdbus introspect \
    --system \
    --dest org.example.Bluetooth \
    --object-path /org/example/Bluetooth

# Monitor D-Bus traffic
dbus-monitor --system

# Inspect GLib/GDBus activity
G_DBUS_DEBUG=message ./your_application

# Inspect process threads
ps -T -p <PID>

# Attach debugger
gdb -p <PID>