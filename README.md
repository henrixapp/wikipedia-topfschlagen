# wikipedia-topfschlagen
A solution to the question, how long you have to travel between wiki articles.

## Build process

Recursivly clone this repository. This is required, because we are using casablanca as http library.

`cmake` is our build tool; Required system packages are:
  * lib-xerces
  * OpenCL

## Usage

### Prep

Get your copy of your favorite wikipedia (https://dumps.wikimedia.org), a file including pages and articles in xml is what you are looking for.
This file is propably compressed. Untar/bz2 it.

call vorbereitung to generate a index of pages and once again for an dictonary of connections between articles.

```sh
./vorbereitung <wiki-dump.xml> wiki.articles
./vorbereitung <wiki-dump.xml> wiki.articles wiki.links
```

The generated files are between 100 MB for index and 280 MB for articles big (german wiki). They are not compressed.

### Interactive Console

To start run `./suchen wiki.articles wiki.links`. Caution: Spaces are written as "_". have fun!

### Server

To start run `./server wiki.articles wiki.links`. You will probably want to use [wikipedia-topfschlagen-web](https://github.com/henrixapp/wikipedia-topfschlagen-web), look there for further instructions.

## Contribution & TODO

* Redirects!
* Docs!

Feel free to make a PR.
