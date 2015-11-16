/**
 * Mandelbulber v2, a 3D fractal generator
 *
 * cQueue - class to manage rendering queue
 *
 * Copyright (C) 2014 Krzysztof Marczak
 *
 * This file is part of Mandelbulber.
 *
 * Mandelbulber is free software: you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * Mandelbulber is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU General Public License for more details. You should have received a copy of the GNU
 * General Public License along with Mandelbulber. If not, see <http://www.gnu.org/licenses/>.
 *
 * Authors: Krzysztof Marczak (buddhi1980@gmail.com), Sebastian Jennen
 */


#ifndef MANDELBULBER2_SRC_QUEUE_HPP_
#define MANDELBULBER2_SRC_QUEUE_HPP_

#include "parameters.hpp"
#include "fractal_container.hpp"
#include "animation_keyframes.hpp"
#include "animation_flight.hpp"
#include "interface.hpp"
#include <QtCore>

class cQueue : public QObject
{
	Q_OBJECT
public:
	enum enumRenderType {
		queue_STILL, queue_FLIGHT, queue_KEYFRAME
	};

	struct structQueueItem {
		structQueueItem(
			QString _filename,
			enumRenderType _renderType):
			filename(_filename), renderType(_renderType) {}
		bool operator==(const structQueueItem& other) const
		{
			return (filename == other.filename && renderType == other.renderType);
		}
		QString filename;
		enumRenderType renderType;
	};

	//initializes queue and create necessary files and folders
	cQueue(cInterface *_interface, const QString &_queueListFileName, const QString &_queueFolder, QObject *parent = 0);
	~cQueue();

	//add settings to queue
	void Append(const QString &filename, enumRenderType renderType = queue_STILL);
	void Append(enumRenderType renderType = queue_STILL);
	void Append(cParameterContainer *par, cFractalContainer *fractPar, cAnimationFrames *frames, cKeyframes *keyframes, enumRenderType renderType = queue_STILL);
	void AppendList(const QString &filename);
	void AppendFolder(const QString &filename);

	//get next queue element into given containers
	bool Get();
	bool Get(cParameterContainer *par, cFractalContainer *fractPar, cAnimationFrames *frames, cKeyframes *keyframes);


	// syncing methods
	QStringList RemoveOrphanedFiles(); //find and delete files which are not on the list
	QStringList AddOrphanedFilesToList(); //add orphaned files from queue folder to the end of the list

	QList<structQueueItem> GetListFromQueueFile(){ return queueListFromFile; } //returns list of fractals to render from queue file
	QStringList GetListFromFileSystem(){ return queueListFileSystem; } //returns list of fractals to render from file system
	QString GetQueueFolder(){ return queueFolder; }

	//get the queue type enum from qstring value
	static enumRenderType GetTypeEnum(const QString &queueText);
	//get the queue type QString from enum value
	static QString GetTypeText(enumRenderType queueType);
	static QString GetTypeName(enumRenderType queueType);
	//get a color for enum value
	static QString GetTypeColor(enumRenderType queueType);

	void SwapQueueItem(int i, int j);//swap queueItem at i with queueItem at j
	void RemoveQueueItem(int i);//remove queue item which is i'th element of list
	void RemoveQueueItem(const structQueueItem queueItem); //remove queue item from list and filesystem
	void UpdateQueueItemType(int i, enumRenderType renderType);

	structQueueItem GetNextFromList(); //gives next filename
	void RemoveFromList(const structQueueItem &queueItem); //remove queue item if it is on the list
	int GetQueueSize();

	bool stopRequest;

signals:
	//request to update table of queue items
	void queueChanged();
	void queueChanged(int i);
	void queueChanged(int i, int j);

public slots:
	void slotQueueRender();

private slots:
	void queueFileChanged(const QString &path);
	void queueFolderChanged(const QString &path);
	void RenderQueue();

	//UI
	void slotQueueAddCurrentSettings();
	void slotQueueAddFromFile();
	void slotQueueAddOrphaned();
	void slotQueueRemoveOrphaned();
	void slotQueueRemoveItem();
	void slotQueueMoveItemUp();
	void slotQueueMoveItemDown();
	void slotQueueTypeChanged(int index);
	void slotQueueListUpdate();
	void slotQueueListUpdate(int i);
	void slotQueueListUpdate(int i, int j);
	void slotStopRequest();
	void slotShowQueueThumbsChanges(int state);

private:
	void AddToList(const structQueueItem &queueItem); //add filename to the end of list
	void RemoveFromFileSystem(const QString &filename); //remove queue file from filesystem
	void StoreList();

	bool ValidateEntry(const QString &filename);

	void UpdateListFromQueueFile(); //updates the list of fractals to render from queue file
	void UpdateListFromFileSystem(); //updates the list of fractals to render from file system

	cInterface *mainInterface;
	Ui::RenderWindow *ui;
	cImage *image;

	QFileSystemWatcher queueFileWatcher;
	QFileSystemWatcher queueFolderWatcher;

	QList<structQueueItem> queueListFromFile;
	QStringList queueListFileSystem;

	QString queueListFileName;
	QString queueFolder;

	RenderedImage *renderedImageWidget;

	QMutex mutex;
};

#endif /* MANDELBULBER2_SRC_QUEUE_HPP_ */
