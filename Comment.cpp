#include <iostream>
#include "Comment.h"

Comment::Comment() {}
Comment::Comment(std::string aauthor, std::string aid, std::string aparent, std::string adate, std::string atext, int alikes, int areplies) {
    author = aauthor;
    id = aid;
    parent = aparent;
    date = adate;
    text = atext;
    likes = alikes;
    replies = areplies;
}

std::string Comment::getAuthor() const {
    return author;
}
std::string Comment::getId() const {
    return id;
}
std::string Comment::getParent() const {
    return parent;
}
std::string Comment::getDate() const {
    return date;
}
std::string Comment::getText() const {
    return text;
}
int Comment::getLikes() const {
    return likes;
}
int Comment::getReplies() const {
    return replies;
}

void Comment::setLikes(int l) {
    likes = l;
}
void Comment::setReplies(int r) {
    replies = r;
}


bool Comment::operator==(const Comment& other) const {
    return id == other.id;
}
