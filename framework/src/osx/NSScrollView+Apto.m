//
//  NSScrollView+Apto.h
//  Apto-OSX
//
//  Created by David on 3/15/13.
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

#import "NSScrollView+Apto.h"

#import "AptoDocumentPositioningView.h"


@implementation NSScrollView (Apto)

- (NSImageAlignment)documentViewAlignment
{
  NSView* documentView = [self documentView];
  
  if ([documentView isKindOfClass:[AptoDocumentPositioningView class]]) {
    return [(AptoDocumentPositioningView*)documentView documentViewAlignment];
  } else {
    // Default NSClipView behavior
    return NSImageAlignBottomLeft;
  }
}

- (void)setDocumentViewAlignment:(NSImageAlignment)value
{
  AptoDocumentPositioningView* documentView = (AptoDocumentPositioningView*)[self documentView];
  
  // If the current document view not a positioning view, create one...
  if (![documentView isKindOfClass:[AptoDocumentPositioningView class]]) {
    AptoDocumentPositioningView* newPositioningView = [[AptoDocumentPositioningView alloc] initWithFrame:[[self contentView] bounds]];
    [self setDocumentView:newPositioningView];
    [newPositioningView setDocumentView:documentView];
    documentView = newPositioningView;
  }
  
  [documentView setDocumentViewAlignment:value];
}

@end
