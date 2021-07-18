#include <gtkmm.h>
#include <gtkmm/drawingarea.h>
#include <iostream>

using namespace Gtk;


class CDrawingArea : public Gtk::DrawingArea
{
public:
	CDrawingArea();
	virtual ~CDrawingArea();
	Cairo::RefPtr<Cairo::Surface> s;

protected:
	bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);
	bool on_event(GdkEvent *gdk_event);
	bool on_configure(GdkEventConfigure* event);
	bool offscreen_render();
	void draw_text(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height);
};

CDrawingArea::CDrawingArea()
{
	signal_draw().connect(sigc::mem_fun(*this, &CDrawingArea::on_draw));
	signal_configure_event().connect(sigc::mem_fun(*this, &CDrawingArea::on_configure));
	add_events( static_cast<Gdk::EventMask>(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK | Gdk::DOUBLE_BUTTON_PRESS | Gdk::BUTTON_MOTION_MASK));
}

bool CDrawingArea::on_event(GdkEvent *gdk_event)
{
	switch (gdk_event->type)
	{
		case GDK_BUTTON_PRESS:
		case GDK_BUTTON_RELEASE:
		case GDK_DOUBLE_BUTTON_PRESS:
		case GDK_MOTION_NOTIFY:
		{
			return true;
		}
	}
	return false;
}

CDrawingArea::~CDrawingArea()
{
	std::cout << "clean" << std::endl;
}

void CDrawingArea::draw_text(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height)
{
	Cairo::RefPtr<Cairo::LinearGradient> linearGradient = Cairo::LinearGradient::create(0, 0, 400, 0);
	linearGradient->add_color_stop_rgba(0.0, 1.00, 0.00, 0.00, 1.0);
	linearGradient->add_color_stop_rgba(1.0, 0.00, 1.00, 0.00, 1.0);
	Cairo::Matrix m = cr->get_matrix();
	m.rotate(3 * M_PI / 2);
	m.translate(400, 0);
	linearGradient->set_matrix(m);
	cr->set_source(linearGradient);
	cr->fill();

	Pango::FontDescription font;
	font.set_family("Monospace");
	//font.set_stretch(Pango::STRETCH_NORMAL);
	font.set_size(30 * PANGO_SCALE);

	auto layout = Pango::Layout::create(cr);
	//auto layout = create_pango_layout();
	layout->set_text("测试一下中文怎么样测试一下中文怎么样测试一下中文怎么样测试一下中文怎么样");
	layout->set_font_description(font);
	layout->set_width(static_cast<int>(width * Pango::SCALE));

	int text_width, text_height;
	layout->get_pixel_size(text_width, text_height);
	cr->move_to((width-text_width)/2, (height-text_height)/2);
	
	layout->show_in_cairo_context(cr);
}

bool CDrawingArea::on_configure(GdkEventConfigure* event)
{
	s = this->get_window()->create_similar_surface(static_cast<Cairo::Content>(CAIRO_CONTENT_COLOR), this->get_width(), this->get_height());
	this->offscreen_render();
}

bool CDrawingArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
	cr->set_source(s, 0, 0);
	cr->paint();
}

bool CDrawingArea::offscreen_render()
{
	if (s && s->get_status() == CAIRO_STATUS_SUCCESS) {
		Cairo::RefPtr<Cairo::Context> cr = Cairo::Context::create(s);
		cr->set_source_rgba(1.f, 1.f, 1.f, 1.f);
		cr->rectangle(0, 0, this->get_width(), this->get_height());
		cr->fill();
		
		draw_text(cr, this->get_width(), this->get_height());
		//cr->fill();
		this->queue_draw();
	}
	return true;
}

int main(int argc, char *argv[])
{
	auto app = Gtk::Application::create(argc, argv, "org.gtkmm.examples.base");
	Gtk::Window window;
	window.set_default_size(400, 400);
	window.show();

	CDrawingArea area;
	window.add(area);
	area.show();

	return app->run(window);
}