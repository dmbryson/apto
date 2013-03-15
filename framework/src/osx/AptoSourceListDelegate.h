//
//  AptoSourceListDelegate.h
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
//  Idea and design inspired by Alex Rozanski's AptoSourceList.  http://perspx.com
//

#import <Cocoa/Cocoa.h>


// Delegate
// --------------------------------------------------------------------------------------------------------------

@protocol AptoSourceListDelegate <NSObject>
@optional
- (BOOL) sourceList:(AptoSourceList*)aSourceList isGroupAlwaysExpanded:(id)group;
- (NSMenu*) sourceList:(AptoSourceList*)aSourceList menuForEvent:(NSEvent*)theEvent item:(id)item;

// NSOutlineViewDelegate
- (BOOL) sourceList:(AptoSourceList*)aSourceList shouldSelectItem:(id)item;
- (NSIndexSet*) sourceList:(AptoSourceList*)aSourceList selectionIndexesForProposedSelection:(NSIndexSet*)proposedSelectionIndexes;

- (BOOL) sourceList:(AptoSourceList*)aSourceList shouldEditItem:(id)item;

- (BOOL) sourceList:(AptoSourceList*)aSourceList shouldTrackCell:(NSCell*)cell forItem:(id)item;

- (BOOL) sourceList:(AptoSourceList*)aSourceList shouldExpandItem:(id)item;
- (BOOL) sourceList:(AptoSourceList*)aSourceList shouldCollapseItem:(id)item;

- (CGFloat) sourceList:(AptoSourceList*)aSourceList heightOfRowByItem:(id)item;

- (NSCell*) sourceList:(AptoSourceList*)aSourceList willDisplayCell:(id)cell forItem:(id)item;
- (NSCell*) sourceList:(AptoSourceList*)aSourceList dataCellForItem:(id)item;

@end


// Notification Methods
// --------------------------------------------------------------------------------------------------------------

@interface NSObject (AptoSourceListNotifications)

// Selection
- (void)sourceListSelectionIsChanging:(NSNotification*)notification;
- (void)sourceListSelectionDidChange:(NSNotification*)notification;

// Item expanding/collapsing
- (void)sourceListItemWillExpand:(NSNotification*)notification;
- (void)sourceListItemDidExpand:(NSNotification*)notification;
- (void)sourceListItemWillCollapse:(NSNotification*)notification;
- (void)sourceListItemDidCollapse:(NSNotification*)notification;

- (void)sourceListDeleteKeyPressedOnRows:(NSNotification*)notification;

@end


// Notification Identifiers
// --------------------------------------------------------------------------------------------------------------
extern NSString* const AptoSLSelectionIsChangingNotification;
extern NSString* const AptoSLSelectionDidChangeNotification;
extern NSString* const AptoSLItemWillExpandNotification;
extern NSString* const AptoSLItemDidExpandNotification;
extern NSString* const AptoSLItemWillCollapseNotification;
extern NSString* const AptoSLItemDidCollapseNotification;
extern NSString* const AptoSLDeleteKeyPressedOnRowsNotification;

