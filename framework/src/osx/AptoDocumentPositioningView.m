//
//  AptoDocumentPositioningView.m
//  Apto
//
//  Created by David Bryson on 3/15/13.
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
//
//  Based on OmniAptKit's OADocumentPositioningView
//
//  Copyright 2002-2005, 2010-2011 Omni Development, Inc.  All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
//  documentation files (the "Software"), to deal in the Software without restriction, including without limitation the
//  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit
//  persons to whom the Software is furnished to do so, subject to the following conditions:
//
//   - Any original copyright notices and this permission notice shall be included in all copies or substantial portions
//     of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
//  WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
//  COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
//  OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#import "AptoDocumentPositioningView.h"



// Private Method Declarations
// --------------------------------------------------------------------------------------------------------------
#pragma mark -

@interface AptoDocumentPositioningView ()
- (void) documentViewFrameChangedNotification:(NSNotification*)notification;
- (void) positionDocumentView;
@end


//  Method Implementations
// --------------------------------------------------------------------------------------------------------------
#pragma mark -

@implementation AptoDocumentPositioningView

@synthesize documentView;

- (void) setDocumentView:(NSView*)value
{
  if (documentView == value) return;
  
  if (documentView != nil) {
    [[NSNotificationCenter defaultCenter] removeObserver:self name:NSViewFrameDidChangeNotification object:documentView];
    [documentView removeFromSuperview];
    documentView = nil;
  }
  
  if (value != nil) {
    documentView = value;
    [self addSubview:documentView];
    [documentView setAutoresizingMask:NSViewNotSizable];
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(documentViewFrameChangedNotification:)
                                                 name:NSViewFrameDidChangeNotification object:documentView];
  }
}


@synthesize documentViewAlignment;

- (void) setDocumentViewAlignment:(NSImageAlignment)value
{
  documentViewAlignment = value;
  [self positionDocumentView];
}



// Initialization
// --------------------------------------------------------------------------------------------------------------
#pragma mark - Initialization

- (id) initWithFrame:(NSRect)frame
{
  self = [super initWithFrame:frame];
  if (self) {
    [self setAutoresizingMask:NSViewWidthSizable | NSViewHeightSizable];
    documentViewAlignment = NSImageAlignCenter;
  }
  
  return self;
}


// NSView
// --------------------------------------------------------------------------------------------------------------
#pragma mark - NSView

- (void) resizeSubviewsWithOldSize:(NSSize)oldFrameSize
{
  [super resizeSubviewsWithOldSize:oldFrameSize];
  [self positionDocumentView];
}


// Notification Handling
// --------------------------------------------------------------------------------------------------------------
#pragma mark - Notification Handling

- (void) documentViewFrameChangedNotification:(NSNotification*)notification
{
  [self positionDocumentView];
  [self setNeedsDisplay:YES];
}


// Layout
// --------------------------------------------------------------------------------------------------------------
#pragma mark - Layout

- (void) positionDocumentView
{
  NSView* superview = [self superview];
  if (superview == nil) return;

  NSSize contentSize = [superview bounds].size;
  NSRect oldDocumentFrame = (documentView == nil) ? NSZeroRect : [documentView frame];
  
  // Ensure that size large enough for the document, but not smaller than the containing scroll view
  NSRect oldFrame = [self frame];
  NSRect newFrame = oldFrame;
  newFrame.size.width = MAX(oldDocumentFrame.size.width, contentSize.width);
  newFrame.size.height = MAX(oldDocumentFrame.size.height, contentSize.height);
  if (!NSEqualRects(newFrame, oldFrame)) {
    [self setFrame:newFrame];
    [[self superview] setNeedsDisplayInRect:NSUnionRect(oldFrame, newFrame)];
  }
  
  if (documentView != nil) {
    NSRect newDocumentFrame = oldDocumentFrame;
    
    // Calculate the desired frame for the current document view alignment setting
    switch (documentViewAlignment) {
      case NSImageAlignCenter:
        newDocumentFrame.origin.x = (newFrame.size.width - newDocumentFrame.size.width) / 2.0f;
        newDocumentFrame.origin.y = (newFrame.size.height - newDocumentFrame.size.height) / 2.0f;
        break;
        
      case NSImageAlignTop:
        newDocumentFrame.origin.x = (newFrame.size.width - newDocumentFrame.size.width) / 2.0f;
        newDocumentFrame.origin.y = (newFrame.size.height - newDocumentFrame.size.height);
        break;
        
      case NSImageAlignTopLeft:
        newDocumentFrame.origin.x = 0.0f;
        newDocumentFrame.origin.y = (newFrame.size.height - newDocumentFrame.size.height);
        break;
        
      case NSImageAlignTopRight:
        newDocumentFrame.origin.x = (newFrame.size.width - newDocumentFrame.size.width);
        newDocumentFrame.origin.y = (newFrame.size.height - newDocumentFrame.size.height);
        break;
        
      case NSImageAlignLeft:
        newDocumentFrame.origin.x = 0.0f;
        newDocumentFrame.origin.y = (newFrame.size.height - newDocumentFrame.size.height) / 2.0f;
        break;
        
      case NSImageAlignBottom:
        newDocumentFrame.origin.x = (newFrame.size.width - newDocumentFrame.size.width) / 2.0f;
        newDocumentFrame.origin.y = 0.0f;
        break;
        
      case NSImageAlignBottomLeft:
        newDocumentFrame.origin.x = 0.0f;
        newDocumentFrame.origin.y = 0.0f;
        break;
        
      case NSImageAlignBottomRight:
        newDocumentFrame.origin.x = (newFrame.size.width - newDocumentFrame.size.width);
        newDocumentFrame.origin.y = 0.0f;
        break;
        
      case NSImageAlignRight:
        newDocumentFrame.origin.x = (newFrame.size.width - newDocumentFrame.size.width);
        newDocumentFrame.origin.y = (newFrame.size.height - newDocumentFrame.size.height) / 2.0f;
        break;
        
      default:
        assert("invalid alignment value in documentViewAlignment");
        break;
    }
    
    // Keep the frame on integral boundaries
    newDocumentFrame.origin.x = (CGFloat)floor(newDocumentFrame.origin.x);
    newDocumentFrame.origin.y = (CGFloat)floor(newDocumentFrame.origin.y);
    
    // If the frame has actually changed, set the new frame and mark the appropriate area as needing to be displayed
    if (!NSEqualPoints(newDocumentFrame.origin, oldDocumentFrame.origin)) {
      [documentView setFrameOrigin:newDocumentFrame.origin];
      [self setNeedsDisplayInRect:NSUnionRect(oldDocumentFrame, newDocumentFrame)];
    }
  }
}

// --------------------------------------------------------------------------------------------------------------
@end
