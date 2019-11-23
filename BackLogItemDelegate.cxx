#include "BackLogItemDelegate.hxx"

#include "TaskItem.hxx"

#include <QPaintEvent>
#include <QPainter>

BackLogItemDelegate::BackLogItemDelegate(QObject* p_parent):
  QItemDelegate(p_parent) {
}

void BackLogItemDelegate::paint(QPainter* p_painter, const QStyleOptionViewItem& p_option, const QModelIndex& p_index) const {

  p_painter->save();

  QStyleOptionViewItem option = setOptions(p_index, p_option);
  p_painter->setFont(option.font);

  QItemDelegate::drawBackground(p_painter, option, p_index);

  p_painter->setRenderHint(QPainter::Antialiasing);
  auto fm = option.fontMetrics;
  auto formerFont = p_painter->font();
  auto formerColor = p_painter->pen().color();

  // Draw title
  auto titleText = p_index.data(BackLogItem::eNameRole).toString();
  auto titleBoundingRect = fm.boundingRect(titleText);
  auto titleRect = p_option.rect.adjusted(10, 0, -p_option.rect.width()+titleBoundingRect.width()+10, 0);
  auto titleFont = formerFont;
  titleFont.setBold(true);
  p_painter->setFont(titleFont);
  p_painter->drawText(titleRect, Qt::AlignCenter, titleText);
  p_painter->setFont(formerFont);

  // Draw epic
  auto epicName = p_index.data(KanbanTaskItem::eEpicRole).toString();
  auto epicBoundingRect = fm.boundingRect(epicName);
  auto epicRect = p_option.rect.adjusted(10+titleRect.width()+5, 3, -p_option.rect.width()+epicBoundingRect.width()+10+titleRect.width()+5+20, -3);
  auto epicColor = p_index.data(Qt::DecorationRole).value<QColor>();
  p_painter->setPen(Qt::NoPen);
  p_painter->setBrush(epicColor);
  p_painter->drawRoundedRect(epicRect, 4, 4);
  p_painter->setBrush(Qt::NoBrush);
  (epicColor.toHsv().value() <= 127)?
    p_painter->setPen(Qt::white):
    p_painter->setPen(formerColor);
  p_painter->drawText(epicRect, Qt::AlignCenter, epicName);

  p_painter->restore();
}
/*
void FindResultItemDelegate::paint(QPainter* p_painter, QStyleOptionViewItem const& p_option,
  QModelIndex const& p_index) const
{
  if (p_index.data(FindResultItem::eIsResultLineRole).toBool() == false)
  {
    return QItemDelegate::paint(p_painter, p_option, p_index);
  }

  p_painter->save();

  QStyleOptionViewItem option = setOptions(p_index, p_option);
  p_painter->setFont(option.font);

  QItemDelegate::drawBackground(p_painter, option, p_index);

  // Check and pixmap rectangles are empty for now
  QRect checkRect;
  QRect pixmapRect;

  // Text
  QRect textRect = option.rect.adjusted(0, 0, checkRect.width() + pixmapRect.width(), 0);

  // Line numbers
  int lineNumberAreaWidth = DrawLineNumber(p_painter, option, textRect, p_index);
  textRect.adjust(lineNumberAreaWidth, 0, 0, 0);

  // Text and focus/selection
  DrawLineText(p_painter, option, textRect, p_index);
  QItemDelegate::drawFocus(p_painter, option, option.rect);

  p_painter->restore();
}

int FindResultItemDelegate::DrawLineNumber(QPainter* p_painter, QStyleOptionViewItem const& p_option,
  QRect const& p_rect, QModelIndex const& p_index) const
{
  static int const LINE_NUMBER_AREA_HORIZONTAL_PADDING = 4;

  auto lineNumber = p_index.data(FindResultItem::eLineNumberRole).toInt();
  Q_ASSERT_X(lineNumber > 0, "FindResultItemDelegate::DrawLineNumber", "Line number starts at 1");

  bool const isSelected = p_option.state & QStyle::State_Selected;
  QString lineText = QString::number(lineNumber);
  int minimumLineNumberDigits = qMax(static_cast<int>(MINIMUM_LINE_NUMBER_DIGITS), lineText.count());
  int digitFontWidth = p_painter->fontMetrics().width(QString(minimumLineNumberDigits, QLatin1Char('0')));
  int lineNumberAreaWidth = LINE_NUMBER_AREA_HORIZONTAL_PADDING + digitFontWidth + LINE_NUMBER_AREA_HORIZONTAL_PADDING;
  QRect lineNumberAreaRect(p_rect);
  lineNumberAreaRect.setWidth(lineNumberAreaWidth);

  QPalette::ColorGroup colorGroup = QPalette::Normal;
  if (p_option.state.testFlag(QStyle::State_Active) == false)
  {
    colorGroup = QPalette::Inactive;
  }
  else if (p_option.state.testFlag(QStyle::State_Enabled) == false)
  {
    colorGroup = QPalette::Disabled;
  }

  p_painter->fillRect(lineNumberAreaRect,
    QBrush(isSelected ?
      p_option.palette.brush(colorGroup, QPalette::Highlight):
      p_option.palette.color(colorGroup, QPalette::Base).darker(110)));

  QStyleOptionViewItem option = p_option;
  option.displayAlignment = Qt::AlignRight | Qt::AlignVCenter;
  option.palette.setColor(colorGroup, QPalette::Text, Qt::darkGray);

  QStyle const* style = QApplication::style();
  int const textMargin = style->pixelMetric(QStyle::PM_FocusFrameHMargin, nullptr, nullptr) + 1;

  QRect const rowRect = lineNumberAreaRect.adjusted(-textMargin, 0, textMargin-LINE_NUMBER_AREA_HORIZONTAL_PADDING, 0);
  QItemDelegate::drawDisplay(p_painter, option, rowRect, lineText);

  return lineNumberAreaWidth;
}

void FindResultItemDelegate::DrawLineText(QPainter* p_painter, QStyleOptionViewItem const& p_option,
  QRect const& p_rect, QModelIndex const& p_index) const
{
  QString text = p_index.data(Qt::DisplayRole).toString();

  int const searchTermStart = p_index.data(FindResultItem::eResultBeginColumnNumberRole).toInt();
  int searchTermLength = p_index.data(FindResultItem::eSearchStringLengthRole).toInt();
  if (searchTermStart < 0 || searchTermStart >= text.length() || searchTermLength < 1)
  {
    QItemDelegate::drawDisplay(p_painter, p_option, p_rect, text);
    return;
  }

  // Clip searchTermLength to end of line
  searchTermLength = qMin(searchTermLength, text.length() - searchTermStart);
  int const textMargin = QApplication::style()->pixelMetric(QStyle::PM_FocusFrameHMargin) + 1;
  QString const textBeforeHighlight = text.left(searchTermStart);
  QString const textUnderHighlight = text.mid(searchTermStart, searchTermLength);
  QString const textAfterHighlight = text.mid(searchTermStart + searchTermLength);
  int searchTermStartPixels = p_painter->fontMetrics().width(textBeforeHighlight);
  int searchTermLengthPixels = p_painter->fontMetrics().width(textUnderHighlight);

  // Rectangles
  QRect beforeHighlightRect(p_rect);
  beforeHighlightRect.setRight(beforeHighlightRect.left() + searchTermStartPixels);

  QRect resultHighlightRect(p_rect);
  resultHighlightRect.setLeft(beforeHighlightRect.right());
  resultHighlightRect.setRight(resultHighlightRect.left() + searchTermLengthPixels);

  QRect afterHighlightRect(p_rect);
  afterHighlightRect.setLeft(resultHighlightRect.right());

  // Draw highlight correctly
  bool isSelected = p_option.state.testFlag(QStyle::State_Selected);
  auto colorGroup = p_option.state.testFlag(QStyle::State_Enabled) ? QPalette::Normal : QPalette::Disabled;
  if (colorGroup == QPalette::Normal && (p_option.state.testFlag(QStyle::State_Active) == false))
  {
    colorGroup = QPalette::Inactive;
  }
  QStyleOptionViewItem baseOption = p_option;
  baseOption.state &= ~QStyle::State_Selected;
  if (isSelected)
  {
    p_painter->fillRect(beforeHighlightRect.adjusted(textMargin, 0, textMargin, 0),
      p_option.palette.brush(colorGroup, QPalette::Highlight));
    p_painter->fillRect(afterHighlightRect.adjusted(textMargin, 0, textMargin, 0),
      p_option.palette.brush(colorGroup, QPalette::Highlight));
  }
  QColor const highlightBackground(Qt::yellow);
  p_painter->fillRect(resultHighlightRect.adjusted(textMargin, 0, textMargin - 1, 0), QBrush(highlightBackground));

  // Text before highlighting
  QStyleOptionViewItem noHighlightOption = baseOption;
  noHighlightOption.rect = beforeHighlightRect;
  noHighlightOption.textElideMode = Qt::ElideNone;
  if (isSelected)
  {
    noHighlightOption.palette.setColor(
      QPalette::Text, noHighlightOption.palette.color(colorGroup, QPalette::HighlightedText));
  }
  else
  {
    noHighlightOption.palette.setColor(QPalette::Text, Qt::gray);
  }
  QItemDelegate::drawDisplay(p_painter, noHighlightOption, beforeHighlightRect, textBeforeHighlight);

  // Highlighted text
  QStyleOptionViewItem highlightOption = noHighlightOption;
  QColor const highlightForeground(QPalette::Text);
  highlightOption.palette.setColor(QPalette::Text, highlightForeground);
  QItemDelegate::drawDisplay(p_painter, highlightOption, resultHighlightRect, textUnderHighlight);

  // Text after Highlighting
  noHighlightOption.rect = afterHighlightRect;
  QItemDelegate::drawDisplay(p_painter, noHighlightOption, afterHighlightRect, textAfterHighlight);
}
 */

QSize BackLogItemDelegate::sizeHint(const QStyleOptionViewItem& p_option, const QModelIndex& p_index) const {
  return QSize(QItemDelegate::sizeHint(p_option, p_index).width(), 30);
}
