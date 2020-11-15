/****************************************************************************
**
** This file is part of the LibreCAD project, a 2D CAD program
**
** Copyright (C) 2010 R. van Twisk (librecad@rvt.dds.nl)
** Copyright (C) 2001-2003 RibbonSoft. All rights reserved.
**
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file gpl-2.0.txt included in the
** packaging of this file.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**
** This copyright notice MUST APPEAR in all copies of the script!
**
**********************************************************************/

#include "rs_actionblocksthumb.h"

#include <QAction>
#include <QApplication>
#include "rs_graphicview.h"
#include "rs_graphic.h"
#include "rs_dialogfactory.h"
#include "rs_insert.h"
#include "rs_painterqt.h"
#include "rs_staticgraphicview.h"
#include "rs_debug.h"
#include "qc_applicationwindow.h"
#include "qg_blockwidget.h"

RS_ActionBlocksThumb::RS_ActionBlocksThumb(
        RS_EntityContainer& container,
        RS_GraphicView& graphicView)
        :RS_ActionInterface("Thumb all Blocks",
                    container, graphicView) {
}

/*recursive add blocks in graphic*/
void RS_ActionBlocksThumb::addBlock(RS_Insert* in, RS_Graphic* g) {

    for (auto e : *in) {

        if (e->rtti() == RS2::EntityInsert) {
            RS_Insert* in = static_cast<RS_Insert*>(e);
            addBlock(in, g);
            g->addBlock(in->getBlockForInsert());
        }
    }
}

//test only, can be deleted.
void RS_ActionBlocksThumb::trigger() {
    RS_DEBUG->print("thumb blocks to pixmap");
    QC_ApplicationWindow* appWindow = QC_ApplicationWindow::getAppWindow();
    if (!appWindow) {
        finish(false);
        return;
    }
    RS_BlockList* bList = appWindow->getBlockWidget()->getBlockList();
    if (bList) {
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        //auto b = bList->getActive();
        for(auto b:*bList) {
            RS_Graphic g(nullptr);
            g.setOwner(false);
            g.getBlockList()->setOwner(false);

            g.clearLayers();
            //           g.addLayer(b->getLayer());
            for (RS_Entity* e = b->firstEntity(RS2::ResolveNone);
                e;
                e = b->nextEntity(RS2::ResolveNone)) {
                g.addEntity(e);
                //if (e->rtti() == RS2::EntityInsert) {
                //    RS_Insert* in = static_cast<RS_Insert*>(e);
                //    g.addBlock(in->getBlockForInsert());
                //    addBlock(in, &g);
                //}
            }

            // set vars for normal pictures and vectors (svg)
            QSize size = b->getThumbSize();
            QSize borders = b->getThumbBorder();
            QPixmap* picture = b->getThumbPixmap();
            // set buffer var
            QPaintDevice* buffer = picture;
            // set painter with buffer
            RS_PainterQt painter(buffer);

            painter.setBackground(Qt::white);
            painter.setDrawingMode(RS2::ModeBW);

            painter.eraseRect(0, 0, size.width(), size.height());

            RS_StaticGraphicView gv(size.width(), size.height(), &painter, &borders);
            gv.setBackground(Qt::white);

            gv.setContainer(&g);
            gv.zoomAuto(false);
            gv.drawEntity(&painter, gv.getContainer());

            // end the picture output
            // GraphicView deletes painter
            painter.end();
        }
        QApplication::restoreOverrideCursor();
        RS_DIALOGFACTORY->commandMessage(tr("Thumb blocks"));
    }
    else {
        RS_DEBUG->print(RS_Debug::D_WARNING,
            "RS_ActionBlocksSave::trigger():  blockList is NULL");
    }
    finish(false);
}



void RS_ActionBlocksThumb::init(int status) {
    RS_ActionInterface::init(status);
    trigger();
}

// EOF
