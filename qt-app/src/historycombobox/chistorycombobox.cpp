#include "chistorycombobox.h"
#include <assert.h>

CHistoryComboBox::CHistoryComboBox(QWidget* parent) :
	QComboBox(parent)
{
	// without this call lineEdit is not created so it would be impossible to connect to it
	setEditable(true);
	// setHistoryMode should only be called after setEditable(true), or lineEdit won't exist yet
	setHistoryMode(true);

	installEventFilter(this);
	lineEdit()->installEventFilter(this);
}

void CHistoryComboBox::setSelectPreviousItemShortcut(const QKeySequence& selectPreviousItemShortcut)
{
	_selectPreviousItemShortcut = selectPreviousItemShortcut;
}

// Enables or disables history mode (moving activated item to the top)
void CHistoryComboBox::setHistoryMode(bool historyMode)
{
	if (historyMode)
		connect(lineEdit(), SIGNAL(returnPressed()), SLOT(currentItemActivated()), Qt::QueuedConnection);
	else
		disconnect(this, SLOT(currentItemActivated()));
}

// Switch to the next combobox item (which means going back through the history if history mode is set)
void CHistoryComboBox::selectPreviousItem()
{
	if (count() <= 0)
		return;

	if (currentText().isEmpty())
		setCurrentIndex(0);
	else if (currentIndex() < count() - 1)
		setCurrentIndex(currentIndex() + 1);

	lineEdit()->selectAll(); // Causes a bug
}

// Set current index to 0 and clear line edit
void CHistoryComboBox::reset()
{
	if (!lineEdit() || count() <= 0)
		return;

	setCurrentIndex(0);
	lineEdit()->clear();
	clearFocus();
}

bool CHistoryComboBox::eventFilter(QObject*, QEvent* e)
{
	if (e->type() == QEvent::KeyPress)
	{
		QKeyEvent * keyEvent = dynamic_cast<QKeyEvent*>(e);
		assert(keyEvent);

		if (keyEvent->text().isEmpty())
			return false;

		QString modifierString;
		if (keyEvent->modifiers() & Qt::ShiftModifier)
			modifierString = "Shift+";
		if (keyEvent->modifiers() & Qt::ControlModifier)
			modifierString = "Ctrl+";
		if (keyEvent->modifiers() & Qt::AltModifier)
			modifierString = "Alt+";
		if (keyEvent->modifiers() & Qt::MetaModifier)
			modifierString = "Meta+";

		QKeySequence fullSequence(modifierString+QKeySequence(keyEvent->key()).toString());
		if (!_selectPreviousItemShortcut.isEmpty() && fullSequence == _selectPreviousItemShortcut)
		{
			selectPreviousItem();
			return true;
		}
	}

	return false;
}

QStringList CHistoryComboBox::items() const
{
	QStringList itemsList;
	for (int i = 0; i < count(); ++i)
		itemsList.push_back(itemText(i));

	return itemsList;
}

//bool CHistoryComboBox::eventFilter(QObject * object, QEvent * e)
//{
//	const int eType = e->type();
//	if ((object != this && object != lineEdit()) || !lineEdit()->hasFocus() || (e->type() != QEvent::KeyPress && e->type() != QEvent::KeyRelease))
//		return false;

//	// FIXME: first return press after starting the program still makes its way to lineedit somehow
//	QKeyEvent * keyEvent = dynamic_cast<QKeyEvent*>(e);
//	assert(e);
//	const int key = keyEvent->key();
//	if (keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return)
//	{
//		if (e->type() == QEvent::KeyPress)
//			emit lineeditReturnPressed();
//		return true;
//	}
//	else if (keyEvent->modifiers() != Qt::NoModifier && !keyEvent->text().isEmpty())
//		return true;

//	return false;
//}

//void CHistoryComboBox::keyPressEvent(QKeyEvent* e)
//{
//	if (!lineEdit()->hasFocus() || e->modifiers() == Qt::NoModifier || e->text().isEmpty())
//		QComboBox::keyPressEvent(e);
//}

// Moves the currently selected item to the top
void CHistoryComboBox::currentItemActivated()
{
	const QString item = itemText(currentIndex());
	emit itemActivated(item);
	removeItem(currentIndex());
	insertItem(0, item);
	setCurrentIndex(0);
	lineEdit()->clear();
}
