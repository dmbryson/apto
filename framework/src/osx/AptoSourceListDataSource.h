//
//  AptoSourceListDataSource.h
//  Apto-OSX
//
//  Created by David on 2/22/13.
//  Copyright 2013 David Michael Bryson. All rights reserved.
//  http://programerror.com/software/apto
//
//  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
//  following conditions are met:
//
//  1.  Redistributions of source code must retain the above copyright notice, this list of conditions and the
//      following disclaimer.
//  2.  Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
//      following disclaimer in the documentation and/or other materials provided with the distribution.
//  3.  Neither the name of David Michael Bryson, nor the names of contributors may be used to endorse or promote
//      products derived from this software without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY DAVID MICHAEL BRYSON AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
//  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//  DISCLAIMED. IN NO EVENT SHALL DAVID MICHAEL BRYSON OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
//  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
//  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
//  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
//  USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//  Authors: David M. Bryson <david@programerror.com>
//
//
//  Idea and design inspired by Alex Rozanski's PXSourceList.  http://perspx.com
//

#import <Cocoa/Cocoa.h>

@class AptoSourceList;


// AptoSourceListDataSource Protocol
// --------------------------------------------------------------------------------------------------------------

@protocol AptoSourceListDataSource <NSObject>

// Required Source List Methods
// --------------------------------------------------------------------------------------------------------------
#pragma mark Required

@required
- (NSUInteger) sourceList:(AptoSourceList*)sourceList numberOfChildrenOfItem:(id)item;
- (id) sourceList:(AptoSourceList*)aSourceList child:(NSUInteger)index ofItem:(id)item;
- (id) sourceList:(AptoSourceList*)aSourceList objectValueForItem:(id)item;
- (BOOL) sourceList:(AptoSourceList*)aSourceList isItemExpandable:(id)item;


// Optional Source List Methods
// --------------------------------------------------------------------------------------------------------------
#pragma mark Optional

@optional
- (void) sourceList:(AptoSourceList*)aSourceList setObjectValue:(id)object forItem:(id)item;

- (BOOL) sourceList:(AptoSourceList*)aSourceList itemHasBadge:(id)item;
- (NSInteger) sourceList:(AptoSourceList*)aSourceList badgeValueForItem:(id)item;
- (NSColor*) sourceList:(AptoSourceList*)aSourceList badgeTextColorForItem:(id)item;
- (NSColor*) sourceList:(AptoSourceList*)aSourceList badgeBackgroundColorForItem:(id)item;

- (BOOL) sourceList:(AptoSourceList*)aSourceList itemHasIcon:(id)item;
- (NSImage*) sourceList:(AptoSourceList*)aSourceList iconForItem:(id)item;


// NSOutlineViewDataSource Protocol
// --------------------------------------------------------------------------------------------------------------
#pragma mark - NSOutlineViewDataSource Protocol

- (id) sourceList:(AptoSourceList*)aSourceList itemForPersistentObject:(id)object;
- (id) sourceList:(AptoSourceList*)aSourceList persistentObjectForItem:(id)item;

- (BOOL) sourceList:(AptoSourceList*)aSourceList writeItems:(NSArray*)items toPasteboard:(NSPasteboard*)pboard;
- (NSDragOperation) sourceList:(AptoSourceList*)sourceList validateDrop:(id<NSDraggingInfo>)info proposedItem:(id)item proposedChildIndex:(NSInteger)index;
- (BOOL) sourceList:(AptoSourceList*)AptoSourceList acceptDrop:(id<NSDraggingInfo>)info item:(id)item childIndex:(NSInteger)index;
- (NSArray*) sourceList:(AptoSourceList*)AptoSourceList namesOfPromisedFilesDroppedAtDestination:(NSURL*)dropDestination forDraggedItems:(NSArray*)items;

- (id<NSPasteboardWriting>) sourceList:(AptoSourceList*)aSourceList pasteboardWriterForItem:(id)item;
- (void) sourceList:(AptoSourceList*)aSourceList draggingSession:(NSDraggingSession*)session willBeginAtPoint:(NSPoint)screenPoint forItems:(NSArray*)draggedItems;
- (void) sourceList:(AptoSourceList*)aSourceList draggingSession:(NSDraggingSession*)session endedAtPoint:(NSPoint)screenPoint operation:(NSDragOperation)operation;
- (void) sourceList:(AptoSourceList*)aSourceList updateDraggingItemsForDrag:(id <NSDraggingInfo>)draggingInfo;

@end
