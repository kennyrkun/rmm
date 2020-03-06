#ifndef VISUAL_WORKSTATION_HPP
#define VISUAL_WORKSTATION_HPP

#include "SFUI/Widget.hpp"
#include "SFUI/Utils/ItemBox.hpp"

class VisualWorkstation : public SFUI::Widget
{
public:
	VisualWorkstation(const sf::String& string);

	enum Status
	{
		LoggedIn,
		NotLoggedIn,
		Unlocked,
		Locked,
	} status = Status::NotLoggedIn;

	void setStatus(Status status);

	// Set the displayed button label
	void setString(const sf::String& string);
	// Return the displayed button label.
	const sf::String& getString() const;

	// callbacks 
	void onStateChanged(SFUI::State state);
	void onMouseMoved(float x, float y);
	void onMouseReleased(float x, float y);
	void onKeyPressed(sf::Keyboard::Key key);
	void onKeyReleased(sf::Keyboard::Key key);

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	SFUI::ItemBox<sf::Text> m_box;

	sf::RectangleShape statusIndicator;
};

#endif // !VISUAL_WORKSTATION_HPP