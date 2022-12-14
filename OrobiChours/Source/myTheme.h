#pragma once
#include <JuceHeader.h>

#define BORDER_WIDTH 2.5f
#define SCALE_TO_KNOB_RATIO 0.85f

class MyLookAndFeel : public LookAndFeel_V4
{
public:
	MyLookAndFeel() {}

	void drawRotarySlider(Graphics& g,
		int x, int y, int width, int height,
		float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle,
		Slider& slider) override
	{
		const auto radius = jmin(width, height) * 0.5f * SCALE_TO_KNOB_RATIO - BORDER_WIDTH * 0.5f;
		const auto centreX = x + width * 0.5f;
		const auto centreY = y + height * 0.5f;
		const auto rx = centreX - radius;
		const auto ry = centreY - radius;
		const auto rw = radius * 2;

		const Colour brightColour = Colour(0xff2b2d31);
		const Colour darkColour = Colour(0xff0d0d11);

		g.setGradientFill(ColourGradient(brightColour, rx, ry, darkColour.brighter(0.05), rx, ry * rw, false));
		g.fillEllipse(rx, ry, rw, rw);

		g.setGradientFill(ColourGradient(brightColour.brighter(0.9), rx, ry, darkColour.darker(0.9), rx, ry + rw, false));
		g.drawEllipse(rx, ry, rw, rw, BORDER_WIDTH);

		Path p;
		const auto pointerLength = radius * 0.33f;
		const auto pointerThickness = 2.0f;
		p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);

		const auto angle = jmap(sliderPosProportional, rotaryStartAngle, rotaryEndAngle);
		p.applyTransform(AffineTransform::rotation(angle).translated(centreX, centreY)); 
		g.setGradientFill(ColourGradient(Colours::grey, rx, ry, Colours::grey.darker(0.2f), rx, ry + rw, false));
		g.fillPath(p);

		Path originalTickShape, t;
		auto tickThickness = 2.0f;
		originalTickShape.addEllipse(-tickThickness * 0.5f, -radius / SCALE_TO_KNOB_RATIO, tickThickness, tickThickness);
		g.setColour(Colours::whitesmoke);

		for (int i = 0; i < numTicks; ++i)
		{
			t = originalTickShape;
			auto normValue = pow((float)i / (numTicks - 1), (float)slider.getSkewFactor());
			float angle = jmap(normValue, rotaryStartAngle, rotaryEndAngle);
			t.applyTransform(AffineTransform::rotation(angle).translated(centreX, centreY));
			g.fillPath(t);
		}

	}

	void setNumTicks(int newValue)
	{
		numTicks = newValue;
	}

private:

	int numTicks = 21;
};