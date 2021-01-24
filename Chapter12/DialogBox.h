//
//  DialogBox.hpp
//  Game-mac
//
//  Created by Michael Lehmann on 24.01.21.
//  Copyright © 2021 Sanjay Madhav. All rights reserved.
//

#ifndef DialogBox_hpp
#define DialogBox_hpp

#include"UIScreen.h"

class DialogBox : public UIScreen
{
public:
    // (Lower draw order corresponds with further back)
    DialogBox(class Game* game, const std::string& text, std::function<void()> onOK);
    ~DialogBox();
};

#endif /* DialogBox_hpp */
