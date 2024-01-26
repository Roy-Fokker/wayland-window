#include <fmt/core.h>
#include <wayland-client.hpp>
#include <wayland-client-protocol-extra.hpp>

auto main() -> int
{
	fmt::println("Hello World! -fmt");

	auto display     = wayland::display_t{};
	auto compositor  = wayland::compositor_t{};
	auto xdg_wm_base = wayland::xdg_wm_base_t{};
	auto registry    = display.get_registry();

	registry.on_global() = [&](uint32_t name, const std::string &interface, uint32_t version) {
		using namespace wayland;

		fmt::println("Found global: {} ", interface);

		if (interface == compositor_t::interface_name)
		{
			registry.bind(name, compositor, version);
		}
		else if (interface == xdg_wm_base_t::interface_name)
		{
			registry.bind(name, xdg_wm_base, version);
		}
	};

	display.roundtrip();

	auto surface = compositor.create_surface();

	auto xdg_surface = xdg_wm_base.get_xdg_surface(surface);

	auto xdg_toplevel = xdg_surface.get_toplevel();
	xdg_toplevel.set_title("Window");

	auto running = true;

	xdg_toplevel.on_close() = [&]() {
		running = false;
	};

	surface.commit();
	display.roundtrip();

	while (running)
	{
		display.dispatch();
	}

	return 0;
}