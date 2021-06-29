#include <gtkmm.h>
#include <gtkmm/drawingarea.h>

using namespace Gtk;

class CDrawingArea : public Gtk::DrawingArea
{
public:
	CDrawingArea();
	virtual ~CDrawingArea();
protected:
	bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height);
};

CDrawingArea::CDrawingArea()
{
	signal_draw().connect(sigc::bind(sigc::mem_fun(*this, &CDrawingArea::on_draw), 300, 300));
}

CDrawingArea::~CDrawingArea()
{
}

bool CDrawingArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height)
{
	int xc, yc;
	xc = width / 2;
	yc = height / 2;

	//cr->set_line_width(10.0);
	cr->rectangle(10, 10, xc, yc);
	cr->stroke();

	cr->arc(xc, yc, width/2, -3.14, 3.14);
	cr->stroke();

	// draw red lines out from the center of the window
	/*cr->set_source_rgb(0.8, 0.0, 0.0);
	cr->move_to(0, 0);
	cr->line_to(xc, yc);
	cr->line_to(0, height);
	cr->move_to(xc, yc);
	cr->line_to(width, yc);
	cr->stroke();*/
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