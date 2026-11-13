#include <glib.h>
#include <glib-unix.h>

#include <unistd.h>
#include <thread>
#include <cstring>

struct AppContext
{
    GMainLoop* loop;
    int fd;
};

gboolean fd_callback(gint fd, GIOCondition condition, gpointer user_data)
{
    auto* context = static_cast<AppContext*>(user_data);

    if (condition & G_IO_IN)
    {
        char buffer[128];
        ssize_t bytes = read(fd, buffer, sizeof(buffer) - 1);

        if (bytes > 0)
        {
            buffer[bytes] = '\0';
            g_print("[MainLoop] received : %s\n", buffer);
        }
        else if (bytes == 0)
        {
            g_print("[MainLoop] Pipe closed\n");
            g_main_loop_quit(context->loop);
            return G_SOURCE_REMOVE;
        }
    }

    if (condition & (G_IO_HUP | G_IO_ERR | G_IO_NVAL))
    {
        g_print("[MainLoop] Pipe hangup or error\n");
        g_main_loop_quit(context->loop);
        return G_SOURCE_REMOVE;
    }

    return G_SOURCE_CONTINUE;
}

int main()
{
    int pipe_fd[2];

    if (pipe(pipe_fd) == -1)
    {
        g_printerr("Failed to create pipe\n");
        return 1;
    }

    GMainLoop* loop = g_main_loop_new(nullptr, FALSE);

    AppContext context{loop, pipe_fd[0]};

    guint source_id = g_unix_fd_add(
        pipe_fd[0],
        static_cast<GIOCondition>(G_IO_IN | G_IO_HUP | G_IO_ERR),
        fd_callback,
        &context
    );

    std::thread writer([&]()
    {
        for (int i = 0; i < 3; i++)
        {
            sleep(2);
            const char* message = "Bluetooth Event\n";
            write(pipe_fd[1], message, strlen(message));
        }

        close(pipe_fd[1]);
    }); 

    g_main_loop_run(loop);

    writer.join();

    // Remove source if still registered
    // In this example it normally removes itself on EOF/HUP.
    // g_source_remove(source_id);

    close(pipe_fd[0]);
    g_main_loop_unref(loop);
    return 0;
}