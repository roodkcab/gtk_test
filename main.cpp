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

	auto layout = create_pango_layout("测试一下中文怎么样测试一下中文怎么样测试一下中文怎么样测试一下中文怎么样");
	layout->set_font_description(font);
	layout->set_width(static_cast<int>(width * Pango::SCALE));

	int text_width;
	int text_height;
	layout->get_pixel_size(text_width, text_height);
	cr->move_to((width-text_width)/2, (height-text_height)/2);
	layout->show_in_cairo_context(cr);
}

bool CDrawingArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
	Cairo::RefPtr<Cairo::LinearGradient> linearGradient = Cairo::LinearGradient::create(0, 0, 400, 0);
	linearGradient->add_color_stop_rgba(0.0, 1.00, 0.00, 0.00, 1.0);
	linearGradient->add_color_stop_rgba(1.0, 0.00, 1.00, 0.00, 1.0);
	Cairo::Matrix m = cr->get_matrix();
	m.rotate(3 * M_PI / 2);
	m.translate(400, 0);
	linearGradient->set_matrix(m);
	cr->set_source(linearGradient);
	cr->rectangle(0, 0, 0, 0);
	draw_text(cr, this->get_width(), this->get_height());
	cr->fill();
}

int main(int argc, char *argv[])
{
	auto app = Gtk::Application::create(argc, argv, "org.gtkmm.examples.base");
	Gtk::Window window;
	window.set_default_size(400, 400);

	CDrawingArea area;
	window.add(area);
	area.show();

	return app->run(window);
}