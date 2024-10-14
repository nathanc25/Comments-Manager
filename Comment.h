#ifndef HW8_COMMENT_H
#define HW8_COMMENT_H
#include <iostream>

class Comment {
public:
    Comment();
    Comment(std::string aauthor, std::string aid, std::string aparent, std::string adate, std::string atext, int alikes, int areplies);

    std::string getAuthor() const;
    std::string getId() const;
    std::string getParent() const;
    std::string getDate() const;
    std::string getText() const;
    int getLikes() const;
    int getReplies() const;

    void setReplies(int areplies);
    void setLikes(int alikes);

    bool operator==(const Comment& other) const;

private:
    std::string author;
    std::string id;
    std::string parent;
    std::string date;
    std::string text;
    int likes;
    int replies;
};

#endif //HW8_COMMENT_H
