//////////////////////////////////基本信息///////////////////////////////////////////////////////  
// ><免责声明 ><  Copyright (c) 2017-2017 by Xie Zhimin All Rights Reserved  
// ><创建日期 ><  2017/03/21  
// ><创建时间 ><  2017年:03月:21日   16时:34分:47秒  
// ><文件     ><  visibility-item-widget.cpp  
// ><文件路径 ><  D:\newSvnCode\OBS\trunk\obs_studio\UI  
// ><隶属工程><   obs-studio  
// ><当前用户 ><  Administrator  
// ><作者     ><  Open Broadcaster Software   
// ><出处     >< 《 https://obsproject.com/ 》  
// ><目的     >< 【】  
// ><设计技术 ><   
// ><         ><  1.  
// ><         ><  2.  
// ><         ><  3.  
// ><         ><  4.  
//////////////////////////////////迭代修改///////////////////////////////////////////////////////  
// ><作者     ><  xzm  
// ><修改日期 ><  2017年:03月:21日   16时:34分:47秒  
// ><原因     ><    
// ><         ><  1.  
// ><         ><  2.  
// ><         ><  3.  
/////////////////////////////////////////////////////////////////////////////////////////////////
#include "visibility-item-widget.hpp"
#include "visibility-checkbox.hpp"
#include "qt-wrappers.hpp"
#include "obs-app.hpp"
#include <QListWidget>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QLabel>

VisibilityItemWidget::VisibilityItemWidget(obs_source_t *source_)
	: source        (source_),
	  enabledSignal (obs_source_get_signal_handler(source), "enable",
	                 OBSSourceEnabled, this),
	  renamedSignal (obs_source_get_signal_handler(source), "rename",
	                 OBSSourceRenamed, this)
{
	const char *name = obs_source_get_name(source);
	bool enabled = obs_source_enabled(source);

	vis = new VisibilityCheckBox();
	vis->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
	/* Fix for non-apple systems where the spacing would be too big */
#ifndef __APPLE__
	vis->setMaximumSize(16, 16);
#endif
	vis->setChecked(enabled);

	label = new QLabel(QT_UTF8(name));
	label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

	QHBoxLayout *itemLayout = new QHBoxLayout();
	itemLayout->addWidget(vis);
	itemLayout->addWidget(label);
	itemLayout->setContentsMargins(5, 2, 5, 2);

	setLayout(itemLayout);
	setStyleSheet("background-color: rgba(255, 255, 255, 0);");

	connect(vis, SIGNAL(clicked(bool)),
			this, SLOT(VisibilityClicked(bool)));
}

VisibilityItemWidget::VisibilityItemWidget(obs_sceneitem_t *item_)
	: item          (item_),
	  source        (obs_sceneitem_get_source(item)),
	  renamedSignal (obs_source_get_signal_handler(source), "rename",
	                 OBSSourceRenamed, this)
{
	const char *name = obs_source_get_name(source);
	bool enabled = obs_sceneitem_visible(item);
	obs_scene_t *scene = obs_sceneitem_get_scene(item);
	obs_source_t *sceneSource = obs_scene_get_source(scene);

	vis = new VisibilityCheckBox();
	vis->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
	/* Fix for non-apple systems where the spacing would be too big */
#ifndef __APPLE__
	vis->setMaximumSize(16, 16);
#endif
	vis->setChecked(enabled);

	label = new QLabel(QT_UTF8(name));
	label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

	QHBoxLayout *itemLayout = new QHBoxLayout();
	itemLayout->addWidget(vis);
	itemLayout->addWidget(label);
	itemLayout->setContentsMargins(5, 2, 5, 2);

	setLayout(itemLayout);
	setStyleSheet("background-color: rgba(255, 255, 255, 0);");

	signal_handler_t *signal = obs_source_get_signal_handler(sceneSource);
	signal_handler_connect(signal, "remove", OBSSceneRemove, this);
	signal_handler_connect(signal, "item_remove", OBSSceneItemRemove,
			this);
	signal_handler_connect(signal, "item_visible", OBSSceneItemVisible,
			this);

	connect(vis, SIGNAL(clicked(bool)),
			this, SLOT(VisibilityClicked(bool)));
}

VisibilityItemWidget::~VisibilityItemWidget()
{
	DisconnectItemSignals();
}

void VisibilityItemWidget::DisconnectItemSignals()
{
	if (!item || sceneRemoved)
		return;

	obs_scene_t *scene = obs_sceneitem_get_scene(item);
	obs_source_t *sceneSource = obs_scene_get_source(scene);
	signal_handler_t *signal = obs_source_get_signal_handler(sceneSource);

	signal_handler_disconnect(signal, "remove", OBSSceneRemove, this);
	signal_handler_disconnect(signal, "item_remove", OBSSceneItemRemove,
			this);
	signal_handler_disconnect(signal, "item_visible", OBSSceneItemVisible,
			this);

	sceneRemoved = true;
}

void VisibilityItemWidget::OBSSceneRemove(void *param, calldata_t *data)
{
	VisibilityItemWidget *window =
		reinterpret_cast<VisibilityItemWidget*>(param);

	window->DisconnectItemSignals();

	UNUSED_PARAMETER(data);
}

void VisibilityItemWidget::OBSSceneItemRemove(void *param, calldata_t *data)
{
	VisibilityItemWidget *window =
		reinterpret_cast<VisibilityItemWidget*>(param);
	obs_sceneitem_t *item = (obs_sceneitem_t*)calldata_ptr(data, "item");

	if (item == window->item)
		window->DisconnectItemSignals();
}

void VisibilityItemWidget::OBSSceneItemVisible(void *param, calldata_t *data)
{
	VisibilityItemWidget *window =
		reinterpret_cast<VisibilityItemWidget*>(param);
	obs_sceneitem_t *curItem = (obs_sceneitem_t*)calldata_ptr(data, "item");
	bool enabled = calldata_bool(data, "visible");

	if (window->item == curItem)
		QMetaObject::invokeMethod(window, "SourceEnabled",
				Q_ARG(bool, enabled));
}

void VisibilityItemWidget::OBSSourceEnabled(void *param, calldata_t *data)
{
	VisibilityItemWidget *window =
		reinterpret_cast<VisibilityItemWidget*>(param);
	bool enabled = calldata_bool(data, "enabled");

	QMetaObject::invokeMethod(window, "SourceEnabled",
			Q_ARG(bool, enabled));
}

void VisibilityItemWidget::OBSSourceRenamed(void *param, calldata_t *data)
{
	VisibilityItemWidget *window =
		reinterpret_cast<VisibilityItemWidget*>(param);
	const char *name = calldata_string(data, "new_name");

	QMetaObject::invokeMethod(window, "SourceRenamed",
			Q_ARG(QString, QT_UTF8(name)));
}

void VisibilityItemWidget::VisibilityClicked(bool visible)
{
	if (item)
		obs_sceneitem_set_visible(item, visible);
	else
		obs_source_set_enabled(source, visible);
}

void VisibilityItemWidget::SourceEnabled(bool enabled)
{
	if (vis->isChecked() != enabled)
		vis->setChecked(enabled);
}

void VisibilityItemWidget::SourceRenamed(QString name)
{
	if (label && name != label->text())
		label->setText(name);
}

void VisibilityItemWidget::SetColor(const QColor &color,
		bool active_, bool selected_)
{
	/* Do not update unless the state has actually changed */
	if (active_ == active && selected_ == selected)
		return;

	QPalette pal = vis->palette();
	pal.setColor(QPalette::WindowText, color);
	vis->setPalette(pal);

	label->setStyleSheet(QString("color: %1;").arg(color.name()));

	active = active_;
	selected = selected_;
}

VisibilityItemDelegate::VisibilityItemDelegate(QObject *parent)
	: QStyledItemDelegate(parent)
{
}

void VisibilityItemDelegate::paint(QPainter *painter,
		const QStyleOptionViewItem &option,
		const QModelIndex &index) const
{
	QStyledItemDelegate::paint(painter, option, index);

	QObject *parentObj = parent();
	QListWidget *list = qobject_cast<QListWidget*>(parentObj);
	if (!list)
		return;

	QListWidgetItem *item = list->item(index.row());
	VisibilityItemWidget *widget =
		qobject_cast<VisibilityItemWidget*>(list->itemWidget(item));
	if (!widget)
		return;

	bool selected = option.state.testFlag(QStyle::State_Selected);
	bool active = option.state.testFlag(QStyle::State_Active);

	QPalette palette = list->palette();
#if defined(_WIN32) || defined(__APPLE__)
	QPalette::ColorGroup group = active ?
		QPalette::Active : QPalette::Inactive;
#else
	QPalette::ColorGroup group = QPalette::Active;
#endif

#ifdef _WIN32
	QPalette::ColorRole highlightRole = QPalette::WindowText;
#else
	QPalette::ColorRole highlightRole = QPalette::HighlightedText;
#endif

	QPalette::ColorRole role;

	if (selected && active)
		role = highlightRole;
	else
		role = QPalette::WindowText;

	widget->SetColor(palette.color(group, role), active, selected);
}

void SetupVisibilityItem(QListWidget *list, QListWidgetItem *item,
		obs_source_t *source)
{
	VisibilityItemWidget *baseWidget = new VisibilityItemWidget(source);

	item->setSizeHint(baseWidget->sizeHint());
	list->setItemWidget(item, baseWidget);
}

void SetupVisibilityItem(QListWidget *list, QListWidgetItem *item,
		obs_sceneitem_t *sceneItem)
{
	VisibilityItemWidget *baseWidget = new VisibilityItemWidget(sceneItem);

	item->setSizeHint(baseWidget->sizeHint());
	list->setItemWidget(item, baseWidget);
}
