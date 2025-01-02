#include <WebKit2/WebKit2.h>
#include <gtk/gtk.h>

// Function to create a simple WebKit-based window
static void on_close(GtkWidget *widget, gpointer data) {
    gtk_main_quit();
}

int main(int argc, char *argv[]) {
    // Initialize GTK
    gtk_init(&argc, &argv);

    // Create a new window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "WebKit Browser");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    
    // Connect window close event to handler
    g_signal_connect(window, "destroy", G_CALLBACK(on_close), NULL);

    // Create a WebKit WebView widget
    WebKitWebView *webview = WEBKIT_WEB_VIEW(webkit_web_view_new());
    gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(webview));

    // Load an initial URL
    webkit_web_view_load_uri(webview, "https://www.example.com");

    // Show all window elements
    gtk_widget_show_all(window);

    // Start the GTK main loop
    gtk_main();

    return 0;
}
