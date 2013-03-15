//
//  AptoSourceList.m
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
//
//
//  Based on Alex Rozanski's PXSourceList.  http://perspx.com
//
//  Copyright 2011 Alex Rozanski. All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
//  following conditions are met:
//
//   - Redistributions of source code must retain the above copyright notice, this list of conditions and the following
//     disclaimer.
//   - Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
//     disclaimer in the documentation and/or other materials provided with the distribution.
//   - Neither the name of the author nor the names of its contributors may be used to endorse or promote products derived
//     from this software without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
//  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
//  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
//  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
//  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
//  USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//
//
//  Authors: David M. Bryson <david@programerror.com>
//

#import "AptoSourceList.h"


// Constants
// --------------------------------------------------------------------------------------------------------------
#pragma mark Constants

// Layout constants
#define MIN_BADGE_WIDTH						22.0		// The minimum badge width for each item (default 22.0)
#define BADGE_HEIGHT							14.0		// The badge height for each item (default 14.0)
#define BADGE_MARGIN							5.0			// The spacing between the badge and the cell for that row
#define ROW_RIGHT_MARGIN					5.0			// The spacing between the right edge of the badge and the edge of the table column
#define ICON_SPACING							2.0			// The spacing between the icon and it's adjacent cell
#define DISCLOSURE_TRIANGLE_SPACE	18.0		// The indentation reserved for disclosure triangles for non-group items

//Drawing constants
#define BADGE_BACKGROUND_COLOR              [NSColor colorWithCalibratedRed:(152/255.0) green:(168/255.0) blue:(202/255.0) alpha:1]
#define BADGE_HIDDEN_BACKGROUND_COLOR       [NSColor colorWithDeviceWhite:(180/255.0) alpha:1]
#define BADGE_SELECTED_TEXT_COLOR           [NSColor keyboardFocusIndicatorColor]
#define BADGE_SELECTED_UNFOCUSED_TEXT_COLOR [NSColor colorWithCalibratedRed:(153/255.0) green:(169/255.0) blue:(203/255.0) alpha:1]
#define BADGE_SELECTED_HIDDEN_TEXT_COLOR		[NSColor colorWithCalibratedWhite:(170/255.0) alpha:1]
#define BADGE_FONT                          [NSFont boldSystemFontOfSize:11]

//Delegate notification constants
NSString* const AptoSLSelectionIsChangingNotification = @"AptoSourceListSelectionIsChanging";
NSString* const AptoSLSelectionDidChangeNotification = @"AptoSourceListSelectionDidChange";
NSString* const AptoSLItemWillExpandNotification = @"AptoSourceListItemWillExpand";
NSString* const AptoSLItemDidExpandNotification = @"AptoSourceListItemDidExpand";
NSString* const AptoSLItemWillCollapseNotification = @"AptoSourceListItemWillCollapse";
NSString* const AptoSLItemDidCollapseNotification = @"AptoSourceListItemDidCollapse";
NSString* const AptoSLDeleteKeyPressedOnRowsNotification = @"AptoSourceListDeleteKeyPressedOnRows";


// Private Method Declarations
// --------------------------------------------------------------------------------------------------------------
#pragma mark -

@interface AptoSourceList ()

- (void) AptoSourceList_setup;

- (NSSize) sizeOfBadgeAtRow:(NSInteger)rowIndex;
- (void) drawBadgeForRow:(NSInteger)rowIndex inRect:(NSRect)badgeFrame;
- (void) registerDelegateToReceiveNotification:(NSString*)notification withSelector:(SEL)selector;

@end


// Public Method Implementations
// --------------------------------------------------------------------------------------------------------------
#pragma mark -

@implementation AptoSourceList

@synthesize iconSize;
@dynamic dataSource;
@dynamic delegate;


// Init/Setup/Dealloc
// --------------------------------------------------------------------------------------------------------------
#pragma mark - Init/Setup/Dealloc

- (AptoSourceList*) initWithCoder:(NSCoder*)decoder
{
  self = [super initWithCoder:decoder];
	if (self) {
    [self AptoSourceList_setup];
	}
	
	return self;
}


- (AptoSourceList*) initWithFrame:(NSRect)frameRect
{
  self = [super initWithFrame:frameRect];
  
  if (self) {
    [self AptoSourceList_setup];
  }
  
  return self;
}


- (void) AptoSourceList_setup
{
  [self setDelegate:(id<AptoSourceListDelegate>)[super delegate]];
  [super setDelegate:self];
  
  [self setDataSource:(id<AptoSourceListDataSource>)[super dataSource]];
  [super setDataSource:self];
  
  iconSize = NSMakeSize(16,16);
}


- (void)dealloc
{
	[super setDataSource:nil];
	[super setDelegate:nil];
	
	[[NSNotificationCenter defaultCenter] removeObserver:slDelegate name:nil object:self];
}



// Custom Accessors
// --------------------------------------------------------------------------------------------------------------
#pragma mark - Custom Accessors

- (void) setDelegate:(id<AptoSourceListDelegate>)aDelegate
{
	// Unregister the old delegate
	[[NSNotificationCenter defaultCenter] removeObserver:slDelegate name:nil object:self];
	
	slDelegate = aDelegate;
	
	// Register the new delegate to receive notifications
	[self registerDelegateToReceiveNotification:AptoSLSelectionIsChangingNotification
                                 withSelector:@selector(sourceListSelectionIsChanging:)];
	[self registerDelegateToReceiveNotification:AptoSLSelectionDidChangeNotification
                                 withSelector:@selector(sourceListSelectionDidChange:)];
	[self registerDelegateToReceiveNotification:AptoSLItemWillExpandNotification
                                 withSelector:@selector(sourceListItemWillExpand:)];
	[self registerDelegateToReceiveNotification:AptoSLItemDidExpandNotification
                                 withSelector:@selector(sourceListItemDidExpand:)];
	[self registerDelegateToReceiveNotification:AptoSLItemWillCollapseNotification
                                 withSelector:@selector(sourceListItemWillCollapse:)];
	[self registerDelegateToReceiveNotification:AptoSLItemDidCollapseNotification
                                 withSelector:@selector(sourceListItemDidCollapse:)];
	[self registerDelegateToReceiveNotification:AptoSLDeleteKeyPressedOnRowsNotification
                                 withSelector:@selector(sourceListDeleteKeyPressedOnRows:)];
}


- (void) setDataSource:(id<AptoSourceListDataSource>)aDataSource
{
	slDataSource = aDataSource;
	
	[self reloadData];
}


- (void) setIconSize:(NSSize)newIconSize
{
	iconSize = newIconSize;
	
	CGFloat rowHeight = [self rowHeight];
	
	// Make sure icon height does not exceed row height. If it does, proportionally constrain icon size.
	if (iconSize.height > rowHeight) {
		iconSize.width = iconSize.width * (rowHeight / iconSize.height);
		iconSize.height = rowHeight;
	}
}


// Data Management
// --------------------------------------------------------------------------------------------------------------
#pragma mark - Data Management

- (void) reloadData
{
	[super reloadData];
	
	// Expand items that are displayed as always expanded
	if ([slDataSource conformsToProtocol:@protocol(AptoSourceListDataSource)] &&
	    [slDelegate respondsToSelector:@selector(sourceList:isGroupAlwaysExpanded:)]) {
    
		for (NSUInteger i = 0; i < [self numberOfGroups]; i++) {
			id item = [slDataSource sourceList:self child:i ofItem:nil];
			
			if ([self isGroupAlwaysExpanded:item]) [self expandItem:item expandChildren:NO];
		}
	}
	
	// If there are selected rows and the item hierarchy has changed, make sure a Group row isn't selected
	if ([self numberOfSelectedRows] > 0) {
		NSIndexSet* selectedIndexes = [self selectedRowIndexes];
		NSUInteger firstSelectedRow = [selectedIndexes firstIndex];
		
		// Is a group item selected?
		if ([self isGroupItem:[self itemAtRow:firstSelectedRow]]) {
			// Work backwards to find the first non-group row
			BOOL foundRow = NO;
			for (NSUInteger i = firstSelectedRow; i > 0; i--) {
				if (![self isGroupItem:[self itemAtRow:i]]) {
					[self selectRowIndexes:[NSIndexSet indexSetWithIndex:i] byExtendingSelection:NO];
					foundRow = YES;
					break;
				}
			}
			
			// If there is no non-group row preceding the selected group item, remove the selection from the List
			if (!foundRow) [self deselectAll:self];
		}
	} else if (![self allowsEmptySelection] && [self numberOfSelectedRows] == 0) {
		// Select the first non-group row if no rows are selected, and empty selection is disallowed
		for (NSInteger i = 0; i < [self numberOfRows]; i++) {
			if (![self isGroupItem:[self itemAtRow:i]]) {
				[self selectRowIndexes:[NSIndexSet indexSetWithIndex:i] byExtendingSelection:NO];
				break;
			}
		}
	}
}


- (NSUInteger) numberOfGroups
{
	if ([slDataSource respondsToSelector:@selector(sourceList:numberOfChildrenOfItem:)]) {
		return [slDataSource sourceList:self numberOfChildrenOfItem:nil];
	}
	
	return 0;
}


- (BOOL) isGroupItem:(id)item
{
	// Groups are defined as root items (at level 0)
	return [self levelForItem:item] == 0;
}


- (BOOL) isGroupAlwaysExpanded:(id)group
{
	// Make sure that the item IS a group to prevent unwanted queries sent to the data source
	if ([self isGroupItem:group]) {
		// Query the data source
		if ([slDelegate respondsToSelector:@selector(sourceList:isGroupAlwaysExpanded:)]) {
			return [slDelegate sourceList:self isGroupAlwaysExpanded:group];
		}
	}
	
	return NO;
}


- (BOOL) itemHasBadge:(id)item
{
	if ([slDataSource respondsToSelector:@selector(sourceList:itemHasBadge:)]) {
		return [slDataSource sourceList:self itemHasBadge:item];
	}
	
	return NO;
}

- (NSInteger) badgeValueForItem:(id)item
{
	//Make sure that the item has a badge
	if (![self itemHasBadge:item]) return NSNotFound;
	
	if ([slDataSource respondsToSelector:@selector(sourceList:badgeValueForItem:)]) {
		return [slDataSource sourceList:self badgeValueForItem:item];
	}
	
	return NSNotFound;
}


// Selection Handling
// --------------------------------------------------------------------------------------------------------------
#pragma mark - Selection Handling

- (void) selectRowIndexes:(NSIndexSet*)indexes byExtendingSelection:(BOOL)extend
{
	NSUInteger numberOfIndexes = [indexes count];
	
	//Prevent empty selection if we don't want it
	if (![self allowsEmptySelection] && numberOfIndexes == 0) return;
	
	// Loop through the indexes and only add non-group row indexes
	// Allows selection across groups without selecting the group rows
	NSMutableIndexSet* newSelectionIndexes = [NSMutableIndexSet indexSet];
  [indexes enumerateRangesUsingBlock:^(NSRange range, BOOL* stop) {
    (void)stop;
    for (NSUInteger idx = 0; idx < range.length; idx++) {
      NSUInteger selectedIndex = idx + range.location;
      if (![self isGroupItem:[self itemAtRow:selectedIndex]]) [newSelectionIndexes addIndex:selectedIndex];
    }
  }];
	
	// If there are any non-group rows selected
	if ([newSelectionIndexes count] > 0) {
		[super selectRowIndexes:newSelectionIndexes byExtendingSelection:extend];
	}
}


// Layout
// --------------------------------------------------------------------------------------------------------------
#pragma mark - Layout

- (NSRect) frameOfOutlineCellAtRow:(NSInteger)row
{
	// Return a zero-rect if the item is always expanded (a disclosure triangle will not be drawn)
	if ([self isGroupAlwaysExpanded:[self itemAtRow:row]]) {
		return NSZeroRect;
	}
  
  NSRect frame = [super frameOfOutlineCellAtRow:row];
  
  if ([self levelForRow:row] > 0) {
    frame.origin.x = [self levelForRow:row] * [self indentationPerLevel];
  }
  
  return frame;
}


- (NSRect) frameOfCellAtColumn:(NSInteger)column row:(NSInteger)row
{
	id item = [self itemAtRow:row];
	
//	NSCell* cell = [self preparedCellAtColumn:column row:row];
//	NSSize cellSize = [cell cellSize];
//	if (!([cell type] == NSImageCellType) && !([cell type] == NSTextCellType)) {
//		cellSize = [cell cellSizeForBounds:[super frameOfCellAtColumn:column row:row]];
//  }
	NSRect cellFrame = [super frameOfCellAtColumn:column row:row];
	
	NSRect rowRect = [self rectOfRow:row];
	
	if ([self isGroupItem:item]) {
		CGFloat minX = NSMinX(cellFrame);
		
		// Set the origin x-coord; if there are no children of the group at current, there will still be a
		// margin to the left of the cell (in cellFrame), which we don't want
		if ([self isGroupAlwaysExpanded:[self itemAtRow:row]]) minX = 7;
		
		return NSMakeRect(minX, NSMidY(cellFrame) - (cellFrame.size.height / 2.0), NSWidth(rowRect) - minX, cellFrame.size.height);
	} else {
		CGFloat leftIndent = [self levelForRow:row] * [self indentationPerLevel] + DISCLOSURE_TRIANGLE_SPACE;
		
		// Calculate space left for a badge if need be
		CGFloat rightIndent = [self sizeOfBadgeAtRow:row].width + ROW_RIGHT_MARGIN;
		
		//Allow space for an icon if need be
		if (![self isGroupItem:item]&&[slDataSource respondsToSelector:@selector(sourceList:itemHasIcon:)]) {
			if([slDataSource sourceList:self itemHasIcon:item]) leftIndent += [self iconSize].width + (ICON_SPACING * 2);
		}
		
		return NSMakeRect(leftIndent, NSMidY(rowRect) - (cellFrame.size.height / 2.0), NSWidth(rowRect) - rightIndent - leftIndent,
                      cellFrame.size.height);
	}
}


// This method calculates and returns the size of the badge for the row index passed to the method. If the
// row for the row index passed to the method does not have a badge, then NSZeroSize is returned.
- (NSSize)sizeOfBadgeAtRow:(NSInteger)rowIndex
{
	id rowItem = [self itemAtRow:rowIndex];
	
	// Make sure that the item has a badge
	if (![self itemHasBadge:rowItem]) return NSZeroSize;
	
	NSAttributedString *badgeAttrString = [[NSAttributedString alloc] initWithString:[NSString stringWithFormat:@"%ld", (long)[self badgeValueForItem:rowItem]]
                                                                        attributes:@{ NSFontAttributeName: BADGE_FONT }];
	
	NSSize stringSize = [badgeAttrString size];
	
	//Calculate the width needed to display the text or the minimum width if it's smaller
	CGFloat width = stringSize.width+(2*BADGE_MARGIN);
	
	if (width < MIN_BADGE_WIDTH) width = MIN_BADGE_WIDTH;
	
	return NSMakeSize(width, BADGE_HEIGHT);
}


- (void) viewDidMoveToSuperview
{
  //If set to YES, this will cause display issues in Lion where the right part of the outline view is cut off
  [self setAutoresizesOutlineColumn:NO];
}


// Drawing
// --------------------------------------------------------------------------------------------------------------
#pragma mark - Drawing

- (void) drawRow:(NSInteger)rowIndex clipRect:(NSRect)clipRect
{
	[super drawRow:rowIndex clipRect:clipRect];
	
	id item = [self itemAtRow:rowIndex];
	
	//Draw an icon if the item has one
	if (![self isGroupItem:item] && [slDataSource respondsToSelector:@selector(sourceList:itemHasIcon:)])	{
		if ([slDataSource sourceList:self itemHasIcon:item]) {
			NSRect cellFrame = [self frameOfCellAtColumn:0 row:rowIndex];
			NSRect iconRect = NSMakeRect(NSMinX(cellFrame) - iconSize.width - ICON_SPACING,
                                   NSMidY(cellFrame) - (iconSize.height / 2.0f),
                                   iconSize.width,
                                   iconSize.height);
			
			if ([slDataSource respondsToSelector:@selector(sourceList:iconForItem:)]) {
				NSImage* icon = [slDataSource sourceList:self iconForItem:item];
				
				if (icon != nil) {
					NSSize actualIconSize = [icon size];
					
					// If the icon is *smaller* than the size retrieved from the -iconSize property, make sure we
					// reduce the size of the rectangle to draw the icon in, so that it is not stretched.
					if ((actualIconSize.width<iconSize.width)||(actualIconSize.height<iconSize.height)) {
						iconRect = NSMakeRect(NSMidX(iconRect)-(actualIconSize.width/2.0f),
                                  NSMidY(iconRect)-(actualIconSize.height/2.0f),
                                  actualIconSize.width,
                                  actualIconSize.height);
					}
					
          [icon drawInRect:iconRect fromRect:NSZeroRect operation:NSCompositeSourceOver fraction:1 respectFlipped:YES hints:nil];
				}
			}
		}
	}
	
	// Draw the badge if the item has one
	if ([self itemHasBadge:item]) {
		NSRect rowRect = [self rectOfRow:rowIndex];
		NSSize badgeSize = [self sizeOfBadgeAtRow:rowIndex];
		
		NSRect badgeFrame = NSMakeRect(NSMaxX(rowRect) - badgeSize.width - ROW_RIGHT_MARGIN,
                                   NSMidY(rowRect) - (badgeSize.height / 2.0),
                                   badgeSize.width,
                                   badgeSize.height);
		
		[self drawBadgeForRow:rowIndex inRect:badgeFrame];
	}
}

- (void) drawBadgeForRow:(NSInteger)rowIndex inRect:(NSRect)badgeFrame
{
	id rowItem = [self itemAtRow:rowIndex];
	
	NSBezierPath *badgePath = [NSBezierPath bezierPathWithRoundedRect:badgeFrame
                                                            xRadius:(BADGE_HEIGHT / 2.0)
                                                            yRadius:(BADGE_HEIGHT / 2.0)];
	
	//Get window and control state to determine colours used
	BOOL isVisible = [[NSApp mainWindow] isVisible];
	BOOL isFocused = [[[self window] firstResponder] isEqual:self];
	NSInteger rowBeingEdited = [self editedRow];
	
	//Set the attributes based on the row state
	NSDictionary* attributes;
	NSColor* backgroundColor;
	
	if ([[self selectedRowIndexes] containsIndex:rowIndex]) {
		backgroundColor = [NSColor whiteColor];
		
		// Set the text color based on window and control state
		NSColor* textColor;
		
		if (isVisible && (isFocused || rowBeingEdited == rowIndex)) {
			textColor = BADGE_SELECTED_TEXT_COLOR;
		} else if (isVisible && !isFocused) {
			textColor = BADGE_SELECTED_UNFOCUSED_TEXT_COLOR;
		} else {
			textColor = BADGE_SELECTED_HIDDEN_TEXT_COLOR;
		}
		
		attributes = @{ NSFontAttributeName: BADGE_FONT, NSForegroundColorAttributeName: textColor };
	}	else {
		//Set the text colour based on window and control state
		NSColor *badgeColor = [NSColor whiteColor];
		
		if (isVisible) {
			//If the data source returns a custom colour..
			if ([slDataSource respondsToSelector:@selector(sourceList:badgeBackgroundColorForItem:)]) {
				backgroundColor = [slDataSource sourceList:self badgeBackgroundColorForItem:rowItem];
				
				if (backgroundColor == nil)	backgroundColor = BADGE_BACKGROUND_COLOR;
			} else { //Otherwise use the default (purple-blue colour)
				backgroundColor = BADGE_BACKGROUND_COLOR;
			}
			
			//If the delegate wants a custom badge text colour..
			if ([slDataSource respondsToSelector:@selector(sourceList:badgeTextColorForItem:)]) {
				badgeColor = [slDataSource sourceList:self badgeTextColorForItem:rowItem];
				
				if (badgeColor == nil)	badgeColor = [NSColor whiteColor];
			}
		}
		else { //Gray colour
			backgroundColor = BADGE_HIDDEN_BACKGROUND_COLOR;
		}
		
		attributes = @{ NSFontAttributeName: BADGE_FONT, NSForegroundColorAttributeName: badgeColor };
	}
	
	[backgroundColor set];
	[badgePath fill];
	
	//Draw the badge text
	NSAttributedString *badgeAttrString = [[NSAttributedString alloc] initWithString:[NSString stringWithFormat:@"%ld", (long)[self badgeValueForItem:rowItem]]
                                                                        attributes:attributes];
	NSSize stringSize = [badgeAttrString size];
	NSPoint badgeTextPoint = NSMakePoint(NSMidX(badgeFrame) - (stringSize.width / 2.0),		// Center in the badge frame
                                       NSMidY(badgeFrame) - (stringSize.height / 2.0));	// Center in the badge frame
	[badgeAttrString drawAtPoint:badgeTextPoint];
}


// Keyboard Handling
// --------------------------------------------------------------------------------------------------------------
#pragma mark - Keyboard Handling

- (void) keyDown:(NSEvent*)theEvent
{
	NSIndexSet* selectedIndexes = [self selectedRowIndexes];
	
	NSString* keyCharacters = [theEvent characters];
	
	//Make sure we have a selection
	if ([selectedIndexes count] > 0) {
		if ([keyCharacters length] > 0) {
			unichar firstKey = [keyCharacters characterAtIndex:0];
			
			if (firstKey == NSUpArrowFunctionKey || firstKey == NSDownArrowFunctionKey) {
				// Handle keyboard navigation across groups
				if ([selectedIndexes count] == 1 && !([theEvent modifierFlags] & NSShiftKeyMask))
				{
					int delta = (firstKey == NSDownArrowFunctionKey) ? 1 : -1;  //Search "backwards" if up arrow, "forwards" if down
					NSInteger newRow = [selectedIndexes firstIndex];
					
					// Keep incrementing/decrementing the row until a non-header row is reached
					do {
						newRow += delta;
						
						//If out of bounds of the number of rows..
						if (newRow < 0 || newRow == [self numberOfRows]) break;
					} while ([self isGroupItem:[self itemAtRow:newRow]]);
					
					
					[self selectRowIndexes:[NSIndexSet indexSetWithIndex:newRow] byExtendingSelection:NO];
					return;
				}
			} else if (firstKey == NSDeleteCharacter || firstKey == NSBackspaceCharacter || firstKey == 0xf728) {
				// Post the notification
				[[NSNotificationCenter defaultCenter] postNotificationName:AptoSLDeleteKeyPressedOnRowsNotification
                                                            object:self
                                                          userInfo:[NSDictionary dictionaryWithObject:selectedIndexes forKey:@"rows"]];
				return;
			}
		}
	}
	
	[super keyDown:theEvent];
}


// Menu Handling
// --------------------------------------------------------------------------------------------------------------
#pragma mark - Menu Handling

- (NSMenu*) menuForEvent:(NSEvent*)theEvent
{
	NSMenu* m = nil;
	if ([slDelegate respondsToSelector:@selector(sourceList:menuForEvent:item:)]) {
		NSPoint clickPoint = [self convertPoint:[theEvent locationInWindow] fromView:nil];
		NSInteger row = [self rowAtPoint:clickPoint];
		id clickedItem = [self itemAtRow:row];
		m = [slDelegate sourceList:self menuForEvent:theEvent item:clickedItem];
	}
	if (m == nil) m = [super menuForEvent:theEvent];
	return m;
}


// NSOutlineView Data Source
// --------------------------------------------------------------------------------------------------------------
#pragma mark - NSOutlineView Data Source

- (NSInteger) outlineView:(NSOutlineView*)outlineView numberOfChildrenOfItem:(id)item
{
  (void)outlineView;
  
	if([slDataSource conformsToProtocol:@protocol(AptoSourceListDataSource)]) {
		return [slDataSource sourceList:self numberOfChildrenOfItem:item];
	}
	
	return 0;
}


- (id) outlineView:(NSOutlineView*)outlineView child:(NSInteger)index ofItem:(id)item
{
  (void)outlineView;
  
	if ([slDataSource conformsToProtocol:@protocol(AptoSourceListDataSource)]) {
		return [slDataSource sourceList:self child:index ofItem:item];
	}
	
	return nil;
}


- (BOOL) outlineView:(NSOutlineView*)outlineView isItemExpandable:(id)item
{
  (void)outlineView;
  
	if ([slDataSource conformsToProtocol:@protocol(AptoSourceListDataSource)]) {
		return [slDataSource sourceList:self isItemExpandable:item];
	}
	
	return NO;
}


- (id) outlineView:(NSOutlineView*)outlineView objectValueForTableColumn:(NSTableColumn*)tableColumn byItem:(id)item
{
  (void)outlineView;
  (void)tableColumn;
  
	if ([slDataSource conformsToProtocol:@protocol(AptoSourceListDataSource)]) {
		return [slDataSource sourceList:self objectValueForItem:item];
	}
	
	return nil;
}


- (void) outlineView:(NSOutlineView*)outlineView setObjectValue:(id)object forTableColumn:(NSTableColumn*)tableColumn byItem:(id)item
{
  (void)outlineView;
  (void)tableColumn;
  
	if ([slDataSource conformsToProtocol:@protocol(AptoSourceListDataSource)]) {
		[slDataSource sourceList:self setObjectValue:object forItem:item];
	}
}


- (id) outlineView:(NSOutlineView*)outlineView itemForPersistentObject:(id)object
{
  (void)outlineView;
  
	if ([slDataSource respondsToSelector:@selector(sourceList:itemForPersistentObject:)]) {
		return [slDataSource sourceList:self itemForPersistentObject:object];
	}
	
	return nil;
}


- (id) outlineView:(NSOutlineView*)outlineView persistentObjectForItem:(id)item
{
  (void)outlineView;
  
	if ([slDataSource respondsToSelector:@selector(sourceList:persistentObjectForItem:)]) {
		return [slDataSource sourceList:self persistentObjectForItem:item];
	}
	
	return nil;
}


- (BOOL) outlineView:(NSOutlineView*)outlineView writeItems:(NSArray*)items toPasteboard:(NSPasteboard*)pasteboard
{
  (void)outlineView;
  
	if ([slDataSource respondsToSelector:@selector(sourceList:writeItems:toPasteboard:)]) {
		return [slDataSource sourceList:self writeItems:items toPasteboard:pasteboard];
	}
	
	return NO;
}


- (NSDragOperation) outlineView:(NSOutlineView*)outlineView validateDrop:(id<NSDraggingInfo>)info proposedItem:(id)item proposedChildIndex:(NSInteger)index
{
  (void)outlineView;
	if ([slDataSource respondsToSelector:@selector(sourceList:validateDrop:proposedItem:proposedChildIndex:)]) {
		return [slDataSource sourceList:self validateDrop:info proposedItem:item proposedChildIndex:index];
	}
	
	return NSDragOperationNone;
}


- (BOOL) outlineView:(NSOutlineView*)outlineView acceptDrop:(id<NSDraggingInfo>)info item:(id)item childIndex:(NSInteger)index
{
  (void)outlineView;
  
	if ([slDataSource respondsToSelector:@selector(sourceList:acceptDrop:item:childIndex:)]) {
		return [slDataSource sourceList:self acceptDrop:info item:item childIndex:index];
	}
	
	return NO;
}


- (NSArray*) outlineView:(NSOutlineView*)outlineView namesOfPromisedFilesDroppedAtDestination:(NSURL*)dropDestination forDraggedItems:(NSArray*)items
{
  (void)outlineView;
  
	if ([slDataSource respondsToSelector:@selector(sourceList:namesOfPromisedFilesDroppedAtDestination:forDraggedItems:)]) {
		return [slDataSource sourceList:self namesOfPromisedFilesDroppedAtDestination:dropDestination forDraggedItems:items];
	}
	
	return nil;
}

- (id<NSPasteboardWriting>) outlineView:(NSOutlineView*)outlineView pasteboardWriterForItem:(id)item
{
  (void)outlineView;
  
  if ([slDataSource respondsToSelector:@selector(sourceList:pasteboardWriterForItem:)]) {
    return [slDataSource sourceList:self pasteboardWriterForItem:item];
  }
  
  return nil;
}


- (void) outlineView:(NSOutlineView*)outlineView draggingSession:(NSDraggingSession*)session willBeginAtPoint:(NSPoint)screenPoint forItems:(NSArray*)draggedItems
{
  (void)outlineView;
  
  if ([slDataSource respondsToSelector:@selector(sourceList:draggingSession:willBeginAtPoint:forItems:)]) {
    return [slDataSource sourceList:self draggingSession:session willBeginAtPoint:screenPoint forItems:draggedItems];
  }
  
}

- (void) outlineView:(NSOutlineView*)outlineView draggingSession:(NSDraggingSession*)session endedAtPoint:(NSPoint)screenPoint operation:(NSDragOperation)operation
{
  (void)outlineView;
  
  if ([slDataSource respondsToSelector:@selector(sourceList:draggingSession:endedAtPoint:operation:)]) {
    return [slDataSource sourceList:self draggingSession:session endedAtPoint:screenPoint operation:operation];
  }
}

- (void) outlineView:(NSOutlineView*)outlineView updateDraggingItemsForDrag:(id<NSDraggingInfo>)draggingInfo
{
  (void)outlineView;
  
  if ([slDataSource respondsToSelector:@selector(sourceList:updateDraggingItemsForDrag:)]) {
    return [slDataSource sourceList:self updateDraggingItemsForDrag:draggingInfo];
  }
}


// NSOutlineView Delegate
// --------------------------------------------------------------------------------------------------------------
#pragma mark - NSOutlineView Delegate

- (BOOL) outlineView:(NSOutlineView*)outlineView shouldExpandItem:(id)item
{
  (void)outlineView;
  
	if ([slDelegate respondsToSelector:@selector(sourceList:shouldExpandItem:)]) {
		return [slDelegate sourceList:self shouldExpandItem:item];
	}
	
	return YES;
}

- (BOOL) outlineView:(NSOutlineView*)outlineView shouldCollapseItem:(id)item
{
  (void)outlineView;
  
	// Make sure the item isn't displayed as always expanded
	if ([self isGroupItem:item] && [self isGroupAlwaysExpanded:item]) return NO;
	
	if ([slDelegate respondsToSelector:@selector(sourceList:shouldCollapseItem:)]) {
		return [slDelegate sourceList:self shouldCollapseItem:item];
	}
	
	return YES;
}

- (NSCell*) outlineView:(NSOutlineView*)outlineView dataCellForTableColumn:(NSTableColumn*)tableColumn item:(id)item
{
  (void)outlineView;
  (void)tableColumn;
  
	if ([slDelegate respondsToSelector:@selector(sourceList:dataCellForItem:)]) {
		return [slDelegate sourceList:self dataCellForItem:item];
	}
	
	NSInteger row = [self rowForItem:item];
	
	// Return the default table column
	return [[[self tableColumns] objectAtIndex:0] dataCellForRow:row];
}

- (void) outlineView:(NSOutlineView*)outlineView willDisplayCell:(id)cell forTableColumn:(NSTableColumn*)tableColumn item:(id)item
{
  (void)outlineView;
  (void)tableColumn;
  
	if ([slDelegate respondsToSelector:@selector(sourceList:willDisplayCell:forItem:)]) {
		[slDelegate sourceList:self willDisplayCell:cell forItem:item];
	}
}

- (BOOL) outlineView:(NSOutlineView*)outlineView shouldSelectItem:(id)item
{
  (void)outlineView;

	// Make sure that the item isn't a group as they can't be selected
	if (![self isGroupItem:item]) {
		if ([slDelegate respondsToSelector:@selector(sourceList:shouldSelectItem:)]) {
			return [slDelegate sourceList:self shouldSelectItem:item];
		}
	}	else {
		return NO;
	}
	
	return YES;
}


- (NSIndexSet*) outlineView:(NSOutlineView*)outlineView selectionIndexesForProposedSelection:(NSIndexSet*)proposedSelectionIndexes
{
  (void)outlineView;

	// The outline view will try to select the first row if -[allowsEmptySelection:] is set to NO – if this is a group row
	// stop it from doing so and leave it to our implementation of-[reloadData] which will select the first non-group row
	// for us.
	if ([self numberOfSelectedRows] == 0 && [self isGroupItem:[self itemAtRow:[proposedSelectionIndexes firstIndex]]]) {
    return [NSIndexSet indexSet];
	}
	
	if ([slDelegate respondsToSelector:@selector(sourceList:selectionIndexesForProposedSelection:)]) {
		return [slDelegate sourceList:self selectionIndexesForProposedSelection:proposedSelectionIndexes];
	}
	
	return proposedSelectionIndexes;
}

- (BOOL) outlineView:(NSOutlineView*)outlineView shouldEditTableColumn:(NSTableColumn*)tableColumn item:(id)item
{
  (void)outlineView;
  (void)tableColumn;

	// Group titles can't be edited
	if ([self isGroupItem:item]) return NO;
	
	if ([slDelegate respondsToSelector:@selector(sourceList:shouldEditItem:)]) {
		return [slDelegate sourceList:self shouldEditItem:item];
	}
	
	return YES;
}


- (BOOL) outlineView:(NSOutlineView*)outlineView shouldTrackCell:(NSCell*)cell forTableColumn:(NSTableColumn*)tableColumn item:(id)item
{
  (void)outlineView;
  (void)tableColumn;

	if ([slDelegate respondsToSelector:@selector(sourceList:shouldTrackCell:forItem:)]) {
		return [slDelegate sourceList:self shouldTrackCell:cell forItem:item];
	}
	
	return NO;
}

- (CGFloat) outlineView:(NSOutlineView*)outlineView heightOfRowByItem:(id)item
{
  (void)outlineView;

  if ([slDelegate respondsToSelector:@selector(sourceList:heightOfRowByItem:)]) {
		return [slDelegate sourceList:self heightOfRowByItem:item];
	}
	
	return [self rowHeight];
}

- (BOOL) outlineView:(NSOutlineView*)outlineView isGroupItem:(id)item
{
  (void)outlineView;

	return [self isGroupItem:item];
}


// Notification Handling
// --------------------------------------------------------------------------------------------------------------
#pragma mark - Notification handling

- (void) outlineViewSelectionIsChanging:(NSNotification*)notification
{
  (void)notification;
  
	[[NSNotificationCenter defaultCenter] postNotificationName:AptoSLSelectionIsChangingNotification object:self];
}


- (void) outlineViewSelectionDidChange:(NSNotification*)notification
{
  (void)notification;
  
	[[NSNotificationCenter defaultCenter] postNotificationName:AptoSLSelectionDidChangeNotification object:self];
}

- (void) outlineViewItemWillExpand:(NSNotification*)notification
{
	[[NSNotificationCenter defaultCenter] postNotificationName:AptoSLItemWillExpandNotification
                                                      object:self
                                                    userInfo:[notification userInfo]];
}

- (void) outlineViewItemDidExpand:(NSNotification*)notification
{
	[[NSNotificationCenter defaultCenter] postNotificationName:AptoSLItemDidExpandNotification
                                                      object:self
                                                    userInfo:[notification userInfo]];
}

- (void) outlineViewItemWillCollapse:(NSNotification*)notification
{
	[[NSNotificationCenter defaultCenter] postNotificationName:AptoSLItemWillCollapseNotification
                                                      object:self
                                                    userInfo:[notification userInfo]];
}

- (void) outlineViewItemDidCollapse:(NSNotification*)notification
{
	[[NSNotificationCenter defaultCenter] postNotificationName:AptoSLItemDidCollapseNotification
                                                      object:self
                                                    userInfo:[notification userInfo]];	
}

- (void) registerDelegateToReceiveNotification:(NSString*)notification withSelector:(SEL)selector
{
	NSNotificationCenter* defaultCenter = [NSNotificationCenter defaultCenter];
	
	// Set the delegate as a receiver of the notification if it implements the notification method
	if ([slDelegate respondsToSelector:selector]) {
		[defaultCenter addObserver:slDelegate selector:selector name:notification object:self];
	}
}

@end
