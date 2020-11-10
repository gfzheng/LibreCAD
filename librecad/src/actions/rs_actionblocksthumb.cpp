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
#include "rs_graphicview.h"
#include "rs_graphic.h"
#include "rs_debug.h"
#include "qc_applicationwindow.h"
#include "qg_blockwidget.h"


RS_ActionBlocksThumb::RS_ActionBlocksThumb(
        RS_EntityContainer& container,
        RS_GraphicView& graphicView)
        :RS_ActionInterface("Thumb all Blocks",
                    container, graphicView) {
}

void RS_ActionBlocksThumb::trigger() {
    RS_DEBUG->print("thumb blocks to pixmap");
    QC_ApplicationWindow* appWindow = QC_ApplicationWindow::getAppWindow();
    if (!appWindow) {
        finish(false);
        return;
    }
    RS_BlockList* bList = appWindow->getBlockWidget()->getBlockList();
    if (bList) {
        //auto b = bList->getActive();
        //if (b) {
        //    RS_Graphic g(nullptr);
        //    g.setOwner(false);
        //    g.getBlockList()->setOwner(false);

        //    g.clearLayers();
        //    //           g.addLayer(b->getLayer());
        //    for (RS_Entity* e = b->firstEntity(RS2::ResolveNone);
        //        e;
        //        e = b->nextEntity(RS2::ResolveNone)) {
        //        g.addEntity(e);
        //        if (e->rtti() == RS2::EntityInsert) {
        //            RS_Insert* in = static_cast<RS_Insert*>(e);
        //            g.addBlock(in->getBlockForInsert());
        //            addBlock(in, &g);
        //        }
        //    }

        //    RS2::FormatType t = RS2::FormatDXFRW;

        //    QG_FileDialog dlg(appWindow->getMDIWindow(), 0, QG_FileDialog::BlockFile);
        //    QString const& fn = dlg.getSaveFile(&t);
        //    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        //    //            g.setModified(true);
        //    g.saveAs(fn, t);
        //    QApplication::restoreOverrideCursor();
        //}
        //else
        //    RS_DIALOGFACTORY->commandMessage(tr("No block activated to save"));
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
