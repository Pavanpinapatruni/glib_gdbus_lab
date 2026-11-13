#include <glib.h>
#include <thread>
#include <string>

struct WorkerContext
{
    GMainLoop* loop;
    guint counter;
};

gboolean worker_callback(gpointer user_data)
{
    WorkerContext* worker = static_cast<WorkerContext*>(user_data);
    worker->counter++;

    g_print("[Worker] Callback %u, threadID: %zu\n",
        worker->counter,
        std::hash<std::thread::id>{}(std::this_thread::get_id())
    );

    if (worker->counter >= 5)
    {
        g_print("[Worker] Stopping custom loop\n");
        g_main_loop_quit(worker->loop);
        return G_SOURCE_REMOVE;
    }

    return G_SOURCE_CONTINUE;
}

void worker_thread_function()
{
    // Create custom context
    GMainContext* context = g_main_context_new();

    // Create loop using custom context
    GMainLoop* loop = g_main_loop_new(context, FALSE);

    WorkerContext worker{loop, 0};

    // Set thread-default context
    g_main_context_push_thread_default(context);

    // Create timer source
    GSource* source = g_timeout_source_new(1000); // 1 second interval

    g_source_set_callback(
        source,
        worker_callback,
        &worker,
        nullptr
    );

    // Attach timer to custom context
    g_source_attach(source, context);

    g_source_unref(source);

    g_print("[Worker] Starting custom main loop \n");

    g_main_loop_run(loop);

    g_print("[Worker] Custom loop stopped\n");

    // Restore previous thread-default context
    g_main_context_pop_thread_default(context);

    g_main_loop_unref(loop);
    g_main_context_unref(context);
}

int main()
{
    g_print("[Main] Application started\n");
    std::thread worker(worker_thread_function);
    g_print("[Main] Worker thread started\n");
    worker.join();
    g_print("[Main] Worker thread finished\n");

    return 0;
}