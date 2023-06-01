#ifndef ENUM_SELECT_H
#define ENUM_SELECT_H

enum SELECTION_ENUM {   // for dialog setMode
    SELECT_DEBTORS = 0,
    SELECT_USERINFO,
    SELECT_BOOKINFO,
};

enum FORMBD_ENUM {  // for formBD setMode
    USER_ADD = 0,
    USER_DELETE,
};

enum FORMGB_ENUM {  //for formBG setMode
    DO_NOTHING = -1,    // if we don't want to change form settings
    BOOK_GET,
    BOOK_BORROW,
    BOOK_EXTENSE
};

#endif // ENUM_SELECT_H
