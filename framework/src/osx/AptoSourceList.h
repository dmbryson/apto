//
//  AptoSourceList.h
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

#import <Apto/AptoSourceListDataSource.h>
#import <Apto/AptoSourceListDelegate.h>


// AptoSourceList Interface
// --------------------------------------------------------------------------------------------------------------

@interface AptoSourceList : NSOutlineView <NSOutlineViewDelegate, NSOutlineViewDataSource> {
  id<AptoSourceListDelegate> slDelegate;
  id<AptoSourceListDataSource> slDataSource;
  
  NSSize iconSize;
}

// Source List Methods
// --------------------------------------------------------------------------------------------------------------
#pragma mark - Source List Methods

- (NSUInteger) numberOfGroups;
- (BOOL) isGroupItem:(id)item;
- (BOOL) isGroupAlwaysExpanded:(id)group;

- (BOOL) itemHasBadge:(id)item;
- (NSInteger) badgeValueForItem:(id)item;


// Properties
// --------------------------------------------------------------------------------------------------------------
#pragma mark - Properties

@property (assign) id<AptoSourceListDataSource> dataSource;
@property (assign) id<AptoSourceListDelegate> delegate;

@property (nonatomic, assign) NSSize iconSize;


// --------------------------------------------------------------------------------------------------------------
@end
