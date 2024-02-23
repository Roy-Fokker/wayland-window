#include <fmt/core.h>

#include <wayland-client.h>

struct wayland_client
{
	wayland_client()
	{
		display = wl_display_connect(nullptr);
		if (display == nullptr)
		{
			fmt::println("Cannot open display");
			throw std::runtime_error("cannot open display");
		}

		auto registry                = wl_display_get_registry(display);
		const auto registry_listener = wl_registry_listener{
			wayland_client::OnRegistry,
			wayland_client::OnRemoveRegistry
		};
		wl_registry_add_listener(registry, &registry_listener, this);
		wl_display_roundtrip(display);
		wl_registry_destroy(registry);

		running = true;
	}

	~wayland_client()
	{
		wl_display_disconnect(display);
	}

	wayland_client(const wayland_client &)                         = default; // copy constructor
	auto operator=(const wayland_client &) -> wayland_client     & = default; // copy assignment
	wayland_client(wayland_client &&) noexcept                     = default; // move constructor
	auto operator=(wayland_client &&) noexcept -> wayland_client & = default; // move assignment

	static void OnRegistry(void *data,
	                       wl_registry *registry,
	                       uint32_t name,
	                       const char *interface,
	                       uint32_t version)
	{
		auto *client = static_cast<wayland_client *>(data);
		if (strcmp(interface, wl_compositor_interface.name) == 0)
		{
			client->compositor = static_cast<wl_compositor *>(
				wl_registry_bind(registry, name, &wl_compositor_interface, version));
		}
		else if (strcmp(interface, wl_shm_interface.name) == 0)
		{
			client->shm = static_cast<wl_shm *>(
				wl_registry_bind(registry, name, &wl_shm_interface, version));
		}
		else if (strcmp(interface, wl_shell_interface.name) == 0)
		{
			client->shell = static_cast<wl_shell *>(
				wl_registry_bind(registry, name, &wl_shell_interface, version));
		}
		else if (strcmp(interface, wl_seat_interface.name) == 0)
		{
			client->seat = static_cast<wl_seat *>(
				wl_registry_bind(registry, name, &wl_seat_interface, version));

			client->pointer = wl_seat_get_pointer(client->seat);

			const struct wl_pointer_listener pointer_listener = {
				OnPointerEnter,
				OnPointerLeave,
				OnPointerMotion,
				OnPointerButton,
				OnPointerAxis
			};
			wl_pointer_add_listener(client->pointer, &pointer_listener, data);
		}
	}

	static void OnRemoveRegistry(void *data,
	                             wl_registry *registry,
	                             uint32_t name)
	{
	}

	static void OnPointerEnter(void *data,
	                           struct wl_pointer *wl_pointer,
	                           uint32_t serial,
	                           struct wl_surface *surface,
	                           wl_fixed_t surface_x,
	                           wl_fixed_t surface_y)
	{
	}
	static void OnPointerLeave(void *data,
	                           struct wl_pointer *wl_pointer,
	                           uint32_t serial,
	                           struct wl_surface *wl_surface)
	{
	}

	static void OnPointerMotion(void *data,
	                            struct wl_pointer *wl_pointer,
	                            uint32_t time,
	                            wl_fixed_t surface_x,
	                            wl_fixed_t surface_y)
	{
	}

	// Program exits if clicking any mouse buttons.
	static void OnPointerButton(void *data,
	                            struct wl_pointer *wl_pointer,
	                            uint32_t serial,
	                            uint32_t time,
	                            uint32_t button,
	                            uint32_t state)
	{
		auto *client    = static_cast<wayland_client *>(data);
		client->running = false;
	}

	static void OnPointerAxis(void *data,
	                          struct wl_pointer *wl_pointer,
	                          uint32_t time,
	                          uint32_t axis,
	                          wl_fixed_t value)
	{
	}

	wl_compositor *compositor = nullptr;
	wl_display *display       = nullptr;
	wl_pointer *pointer       = nullptr;
	wl_seat *seat             = nullptr;
	wl_shell *shell           = nullptr;
	wl_shm *shm               = nullptr;
	bool running              = false;
};

struct wayland_window
{
	wayland_window()
	{
		client = std::make_unique<wayland_client>();
		create_buffer();
		create_surface();
		redraw();
	}

	~wayland_window()
	{
		wl_buffer_destroy(buffer);
		wl_shell_surface_destroy(shell_surface);
		wl_surface_destroy(surface);
	}

	auto is_running() -> bool
	{
		auto ret = 0;
		if (client->running)
		{
			ret = wl_display_dispatch(client->display);
		}

		return client->running and (ret != -1);
	}

	void create_buffer()
	{
	}

	void create_surface()
	{
		surface = wl_compositor_create_surface(client->compositor);
		if (surface == nullptr)
		{
			fmt::println("cannot create surface");
			throw std::runtime_error("no surface");
		}

		shell_surface = wl_shell_get_shell_surface(client->shell, surface);
		if (shell_surface == nullptr)
		{
			fmt::println("cannot create shell surface");
			throw std::runtime_error("no shell surface");
		}

		wl_shell_surface_set_toplevel(shell_surface);
		wl_shell_surface_set_user_data(shell_surface, surface);
		wl_surface_set_user_data(surface, nullptr);
		wl_shell_surface_set_title(shell_surface, "Wayland Window");
	}

	void redraw()
	{
	}

	std::unique_ptr<wayland_client> client{ nullptr };
	wl_shell_surface *shell_surface = nullptr;
	wl_surface *surface             = nullptr;
	wl_buffer *buffer               = nullptr;
	void *data                      = nullptr;
	wl_callback *callback           = nullptr;
};

auto main() -> int
{
	fmt::println("Hello World! -fmt");

	auto window = wayland_window();

	auto ret = 0;
	while (window.is_running())
	{
	}

	return 0;
}