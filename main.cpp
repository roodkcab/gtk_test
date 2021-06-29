#include <gtkmm.h>
#include <gtkmm/drawingarea.h>
#include <iostream>

using namespace Gtk;

class CDrawingArea : public Gtk::DrawingArea
{
public:
	CDrawingArea();
	virtual ~CDrawingArea();
protected:
	bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);
	void draw_text(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height);
};

CDrawingArea::CDrawingArea()
{
	signal_draw().connect(sigc::mem_fun(*this, &CDrawingArea::on_draw));
}

CDrawingArea::~CDrawingArea()
{
	std::cout << "clean" << std::endl;
}

void CDrawingArea::draw_text(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height)
{
	Pango::FontDescription font;
	font.set_family("Monospace");
	//font.set_stretch(Pango::STRETCH_NORMAL);
	font.set_size(30 * PANGO_SCALE);

	auto layout = create_pango_layout("Hi there!");
	layout->set_font_description(font);

	int text_width;
	int text_height;
	layout->get_pixel_size(text_width, text_height);
	cr->move_to((width-text_width)/2, (height-text_height)/2);
	layout->show_in_cairo_context(cr);
}

bool CDrawingArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
	int width = this->get_width();
	int height = this->get_height();
	int xc, yc;
	xc = width / 2;
	yc = height / 2;
	cr->move_to(10, 10);
	cr->line_to(200, 10);
	cr->line_to(200, 100);
	cr->line_to(10, 100);
	cr->line_to(10, 10);
	cr->set_source_rgba(0.8, 0, 0, 1);
	cr->fill_preserve();
	cr->set_line_width(10);
	cr->set_source_rgba(0.8, 0.8, 0, 1);
	cr->stroke();

	cr->arc(xc, yc, width/2, -3.14, 3.14);
	cr->stroke();
}

int main(int argc, char *argv[])
{
	auto app = Gtk::Application::create(argc, argv, "org.gtkmm.examples.base");
	Gtk::Window window;
	window.set_default_size(300, 300);

	CDrawingArea area;
	window.add(area);
	area.show();

	return app->run(window);
}