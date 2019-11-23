#include "KanbanItemDelegate.hxx"

#include "TaskItem.hxx"

#include <QPainter>

KanbanItemDelegate::KanbanItemDelegate(QObject* p_parent):
  QItemDelegate(p_parent) {
}

void KanbanItemDelegate::paint(QPainter* p_painter, const QStyleOptionViewItem& p_option, const QModelIndex& p_index) const {

  p_painter->save();

  QStyleOptionViewItem option = setOptions(p_index, p_option);
  p_painter->setFont(option.font);

  QItemDelegate::drawBackground(p_painter, option, p_index);

  if (p_index.data(KanbanTaskItem::eIsTaskRole).toBool()) {
    p_painter->setRenderHint(QPainter::Antialiasing);

    auto formerColor = p_painter->pen().color();
    auto formerFont = p_painter->font();
    QColor statusColor[static_cast<int>(TaskStatus::eNone)] = {
        QColor("#d35400"),
        QColor("#f1c40f"),
        QColor("#60a917"),
        QColor("#1ba1e2")
    };

    // Draw background
    auto backgroundRect = p_option.rect.adjusted(5, 5, -5, -5);
    p_painter->fillRect(backgroundRect, QColor("#cccccc"));

    // Draw card status
    auto cardRect = p_option.rect.adjusted(14, 10, -10, -10);
    auto cardStatusRect = p_option.rect.adjusted(10, 10, -cardRect.width()+4, -10);
    p_painter->setPen(Qt::NoPen);
    p_painter->setBrush(statusColor[p_index.data(KanbanTaskItem::eStatusRole).toInt()]);
    p_painter->drawRoundedRect(cardStatusRect, 4, 4);

    // Draw card
    p_painter->setPen(Qt::NoPen);
    p_painter->setBrush(QColor("#333333"));
    auto cacheRect = cardRect.adjusted(0, 0, -cardRect.width()+10, 0);
    p_painter->drawRect(cacheRect);
    p_painter->drawRoundedRect(cardRect, 4, 4);

    // Draw card border
    auto cardBorderRect = p_option.rect.adjusted(10, 10, -10, -10);
    p_painter->setPen(QColor("#eeeeee"));
    p_painter->setBrush(Qt::NoBrush);
    p_painter->drawRoundedRect(cardBorderRect, 4, 4);

    // Draw title
    auto titleRect = p_option.rect.adjusted(25, 20, -20, -cardRect.height()+20+10);
    auto titleText = p_index.data(KanbanTaskItem::eNameRole).toString();
    p_painter->setPen(Qt::white);
    auto titleFont = formerFont;
    titleFont.setBold(true);
    p_painter->setFont(titleFont);
    p_painter->drawText(titleRect, Qt::AlignVCenter | Qt::AlignLeft, titleText);
    p_painter->setFont(formerFont);

    // Draw description
    auto descriptionRectangle = p_option.rect.adjusted(25, 10 + 20 + titleRect.height(), -20, -70);
    auto descriptionText = p_index.data(KanbanTaskItem::eDescriptionRole).toString();
    p_painter->drawText(descriptionRectangle, Qt::AlignTop | Qt::AlignLeft, descriptionText);

    // Draw priority
    auto priorityRect = p_option.rect.adjusted(25, 10+20+titleRect.height()+descriptionRectangle.height()+5, -20, -45);
    auto priority = QString("#%1").arg(p_index.data(KanbanTaskItem::ePriorityRole).toInt());
    p_painter->drawText(priorityRect, Qt::AlignVCenter | Qt::AlignLeft, priority);

    // Draw epic
    auto fm = option.fontMetrics;
    auto epicName = p_index.data(KanbanTaskItem::eEpicRole).toString();
    auto epicBoundingRect = fm.boundingRect(epicName);
    auto epicRect = p_option.rect.adjusted(25, 10+20+titleRect.height()+descriptionRectangle.height()+30, -cardRect.width()+20+epicBoundingRect.width(), -20);
    auto epicColor = p_index.data(Qt::DecorationRole).value<QColor>();
    p_painter->setPen(Qt::NoPen);
    p_painter->setBrush(epicColor);
    p_painter->drawRoundedRect(epicRect, 4, 4);
    p_painter->setBrush(Qt::NoBrush);
    (epicColor.toHsv().value() <= 127)?
      p_painter->setPen(Qt::white):
      p_painter->setPen(formerColor);
    p_painter->drawText(epicRect, Qt::AlignCenter, epicName);

  } else {
    auto formerColor = p_painter->pen().color();

    // Draw background
    auto epicColor = p_index.data(Qt::DecorationRole).value<QColor>();
    if (epicColor.isValid())
      p_painter->fillRect(p_option.rect, epicColor);

    // Draw expand symbole
    auto expandBR = p_option.rect.adjusted(7, p_option.rect.height()/2-5, -p_option.rect.width()+7+10, -p_option.rect.height()/2+5);
    QPoint A, B, C;
    A = expandBR.topLeft();
    if (p_index.data(KanbanEpicItem::eExpandedRole).toBool()) {
      B = expandBR.center();
      B.setY(B.y()+expandBR.height()/2);
      C = expandBR.topRight();
    } else {
      B = expandBR.center();
      B.setX(B.x()+expandBR.width()/2);
      C = expandBR.bottomLeft();
    }
    QPainterPath path;
    path.moveTo(A);
    path.lineTo(B);
    path.lineTo(C);
    p_painter->setBrush(Qt::NoBrush);
    QPen pen(formerColor, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    if (epicColor.toHsv().value() <= 127)
      pen.setColor(Qt::white);
    p_painter->setPen(pen);
    p_painter->drawPath(path);

    // Draw epic name
    auto font = p_painter->font();
    auto textRect = p_option.rect.adjusted(25, 0, 0, 0);
    font.setBold(true);
    p_painter->setFont(font);
    p_painter->drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, p_index.data().toString());
  }

  p_painter->restore();
}

QSize KanbanItemDelegate::sizeHint(QStyleOptionViewItem const& p_option, QModelIndex const& p_index) const {
  auto normalSizeHint = QItemDelegate::sizeHint(p_option, p_index);

  if (p_index.data(KanbanTaskItem::eIsTaskRole).toBool())
    return QSize(normalSizeHint.width(), 300);
  else
    return QSize(normalSizeHint.width(), 40);
}
