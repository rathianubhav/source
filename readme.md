<!--
*** Thanks for checking out this README Template. If you have a suggestion that would
*** make this better, please fork the repo and create a pull request or simply open
*** an issue with the tag "enhancement".
*** Thanks again! Now go create something AMAZING! :D
-->





<!-- PROJECT SHIELDS -->
<!--
*** I'm using markdown "reference style" links for readability.
*** Reference links are enclosed in brackets [ ] instead of parentheses ( ).
*** See the bottom of this document for the declaration of the reference variables
*** for contributors-url, forks-url, etc. This is an optional, concise syntax you may use.
*** https://www.markdownguide.org/basic-syntax/#reference-style-links
-->
[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![MIT License][license-shield]][license-url]
[![LinkedIn][linkedin-shield]][linkedin-url]



<!-- PROJECT LOGO -->
<br />
<p align="center">
  <a href="https://github.com/itsmanjeet/source.git">
    <img src="data/logo.svg" alt="Logo" width="80" height="80">
  </a>

  <h3 align="center">Source</h3>

  <p align="center">
    A Memory Safe programming language compatiable to 'C' codes.
    <br />
    <a href="https://github.com/itsmanjeet/source"><strong>Explore the docs »</strong></a>
    <br />
    <br />
    <a href="https://github.com/itsmanjeet/source">View Demo</a>
    ·
    <a href="https://github.com/itsmanjeet/source/issues">Report Bug</a>
    ·
    <a href="https://github.com/itsmanjeet/source/issues">Request Feature</a>
  </p>
</p>



<!-- TABLE OF CONTENTS -->
## Table of Contents

* [About the Project](#about-the-project)
  * [Built With](#built-with)
* [Getting Started](#getting-started)
  * [Prerequisites](#prerequisites)
  * [Installation](#installation)
* [Usage](#usage)
* [Roadmap](#roadmap)
* [Contributing](#contributing)
* [License](#license)
* [Contact](#contact)
* [Acknowledgements](#acknowledgements)




<!-- GETTING STARTED -->
## Getting Started

Source aims to be a memory safe programming language with compatiability to 'C' codes. Its a experimental project and might not suitable to production use. Main focus of this project is to define the pointers and array of 'C' language which is direct or indirect root cause of every memory vurnability issues.

### Prerequisites

Source will be pre install in [rlxos](https://releax.in/) but can compile in on any linux/unix or windows operating system. Source uses only standard library and STL of C++, no any external dependencies


### Compilation
```bash
    meson build . --prefix=/usr
    ninja -C build
```

### Installation
``` bash
    ninja -C build . install
```

<!-- USAGE EXAMPLES -->
## Usage
Source use common and standard command line arguments for performing operations, like
```bash
    source <file>
```

```bash
    Source 0.1.0 

    Usage: Source [options] <file>

    Options:
        lex                   stdout lexer output
        label                 stdout label of ast
        --debug               enable full debug
        --debug-parser        enable debugging messages for parser (DEBUG=full)
```

_For more examples, please refer to the [Documentation](https://github.com/itsmanjeet/source/docs/Source.html)_



<!-- ROADMAP -->
## Roadmap

See the [open issues](https://github.com/itsmanjeet/source/issues) for a list of proposed features (and known issues).



<!-- CONTRIBUTING -->
## Contributing

Contributions are what make the open source community such an amazing place to be learn, inspire, and create. Any contributions you make are **greatly appreciated**.

1. Fork the Project
2. Create your Feature Branch (`git checkout -b 0.1.0`)
3. Commit your Changes (`git commit -m '[my-id] my awesome Feature'`)
4. Push to the Branch (`git push origin 0.1.0`)
5. Open a Pull Request



<!-- LICENSE -->
## License

Distributed under the GPL3 License. See `license` for more information.



<!-- CONTACT -->
## Contact
Manjeet Singh - [@releaxos](https://twitter.com/releaxos) - itsmanjeet@releax.in
Anubhav Rathi - anubhavrathi1999@gmail.com

[Join](https://discord.gg/TXTxDTYcdg) our discord server for any query


Project Link: [https://github.com/itsmanjeet/source](https://github.com/itsmanjeet/source)



<!-- ACKNOWLEDGEMENTS -->
## Acknowledgements
* [Scratch](https://github.com/venomlinux/scratchpkg)





<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/itsmanjeet/source.svg?style=flat-square
[contributors-url]: https://github.com/itsmanjeet/source/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/itsmanjeet/source.svg?style=flat-square
[forks-url]: https://github.com/itsmanjeet/source/network/members
[stars-shield]: https://img.shields.io/github/stars/itsmanjeet/source.svg?style=flat-square
[stars-url]: https://github.com/itsmanjeet/source/stargazers
[issues-shield]: https://img.shields.io/github/issues/itsmanjeet/source.svg?style=flat-square
[issues-url]: https://github.com/itsmanjeet/source/issues
[license-shield]: https://img.shields.io/github/license/itsmanjeet/source.svg?style=flat-square
[license-url]: https://github.com/itsmanjeet/source/blob/master/license
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=flat-square&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/releax
