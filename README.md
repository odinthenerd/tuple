# tuple
standard conforming compressed tuple implementation

# Rational
All three major std::tuple implementations currently sore the elements either in order oder in reverse order in memory. Since different elements may have different allignment this may leed to padding between elements and thus a larger total size. 

Although I am no expert at interpreting the relevant standardese my reading supports the idea that the order of initialization, destruction and layout are not specified. The fact that some STL implementations use reverse order also suggests that no one is relying on the layout while maintaining cross platform support. There is also a discussion [https://gcc.gnu.org/bugzilla/show_bug.cgi?id=66699](here) which also supports this idea.

# Goals
By sorting the tuple elements by allignment and allowing empty base class optimization of stateless elements I hope to shrink the size of tuples which contain empty elements or elements of different allignment. 

Use modern alias based template metaprogramming techniques I hope to keep compile time similar to the current implementations despite the greater work load which comes with sorting and swizzling the types.
