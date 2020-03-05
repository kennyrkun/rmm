#include "VisualWorkstation.hpp"

VisualWorkstation::VisualWorkstation(const sf::String& string)
{
	m_box.item().setFont(SFUI::Theme::getFont());
	m_box.item().setCharacterSize(SFUI::Theme::textCharacterSize);
	m_box.setSize(SFUI::Theme::minWidgetWidth, SFUI::Theme::getBoxHeight());
	setString(string);

	statusIndicator.setSize(sf::Vector2f(m_box.getSize().y, m_box.getSize().y));
	statusIndicator.setFillColor(sf::Color::Red);

	// TODO: find a better place to put this
	m_box.setPosition(statusIndicator.getSize().x + SFUI::Theme::MARGIN, 0);

	setSize(sf::Vector2f(m_box.getSize().x + statusIndicator.getSize().x + SFUI::Theme::MARGIN, m_box.getSize().y));
}

void VisualWorkstation::setStatus(Status status)
{
	switch (status)
	{
	case VisualWorkstation::Status::Inactive:
		statusIndicator.setFillColor(sf::Color::Red);
		break;
	case VisualWorkstation::Status::Locked:
		statusIndicator.setFillColor(sf::Color::Yellow);
		break;
	case VisualWorkstation::Status::Active:
		statusIndicator.setFillColor(sf::Color::Green);
		break;
	default:
		break;
	}
}

void VisualWorkstation::setString(const sf::String& string)
{
	m_box.item().setString(string);

	// Recompute widget width
	float fittingWidth = m_box.item().getLocalBounds().width + SFUI::Theme::PADDING * 2.0f + SFUI::Theme::borderSize * 2.0f;
	float width = std::max(fittingWidth, SFUI::Theme::minWidgetWidth);

	m_box.setSize(width, SFUI::Theme::getBoxHeight());
	m_box.centerText(m_box.item());

	m_box.setPosition(statusIndicator.getSize().x + SFUI::Theme::MARGIN, 0);

	setSize(sf::Vector2f(m_box.getSize().x + statusIndicator.getSize().x + SFUI::Theme::MARGIN, m_box.getSize().y));
}

const sf::String& VisualWorkstation::getString() const
{
	return m_box.item().getString();
}

void VisualWorkstation::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(m_box, states);
	target.draw(statusIndicator, states);
}

// Callbacks

void VisualWorkstation::onStateChanged(SFUI::State state)
{
	m_box.applyState(state);
}

void VisualWorkstation::onMouseMoved(float x, float y)
{
	if (getState() == SFUI::State::Pressed)
	{
		if (containsPoint(sf::Vector2f(x, y)))
			m_box.press();
		else
			m_box.release();
	}
}

void VisualWorkstation::onMouseReleased(float x, float y)
{
	// TODO: allow changing triggerCallback on mouseRelease or mousePress

	// FIXME: issue #3
//	std::cout << x << std::endl;
//	std::cout << y << std::endl;

	if (containsPoint(sf::Vector2f(x, y)))
		triggerCallback();
}

void VisualWorkstation::onKeyPressed(sf::Keyboard::Key key)
{
	if (key == sf::Keyboard::Return)
	{
		triggerCallback();
		m_box.press();
	}
}

void VisualWorkstation::onKeyReleased(sf::Keyboard::Key key)
{
	if (key == sf::Keyboard::Return)
		m_box.release();
}
