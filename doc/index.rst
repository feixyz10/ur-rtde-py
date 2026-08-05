ur_rtde
=======
|License badge| |Build badge|

.. |License badge| image:: https://img.shields.io/badge/License-MIT-yellow.svg
   :target: https://opensource.org/licenses/MIT

.. |Build badge| image:: https://gitlab.com/sdurobotics/ur_rtde/badges/master/pipeline.svg
   :target: https://gitlab.com/sdurobotics/ur_rtde/commits/master

A cross-platform C++ interface for controlling Universal Robot (UR)
manipulators. This interface is capable of controlling all UR
robots that facilitates a `Real-Time Data Exchange (RTDE)
<https://www.universal-robots.com/how-tos-and-faqs/how-to/ur-how-tos/real-time-data-exchange-rtde-guide-22229/>`_. ur_rtde can be used both from C++ and Python with bindings.

In this documentation
---------------------

..  grid:: 1 1 2 2

   ..  grid-item:: :doc:`Getting started <pages/getting_started/index>`

       **Start here**: a hands-on introduction to ur_rtde for new users

   ..  grid-item:: :doc:`Code Examples <pages/examples/index>`

       **Ready-to-run C++ and Python scripts**  for common robot control tasks.

.. grid:: 1 1 2 2
   :reverse:

   .. grid-item:: :doc:`Reference <pages/reference/index>`

      **Technical information** - specifications, APIs

   .. grid-item:: :doc:`Core Concepts & System Architecture <pages/explanation/index>`

      **Foundational knowledge** on RTDE timing, interface design, and OS configuration.

---------

How to Cite
^^^^^^^^^^^
If you use ur_rtde in your research, please cite this `paper <https://doi.org/10.1109/SII59315.2025.10871000>`__:


..  code-block:: latex

    @inproceedings{10871000,
      author={Lindvig, Anders Prier and Iturrate, Iñigo and Kindler, Uwe and Sloth, Christoffer},
      booktitle={2025 IEEE/SICE International Symposium on System Integration (SII)},
      title={ur_rtde: An Interface for Controlling Universal Robots (UR) using the
      Real-Time Data Exchange(RTDE)}, year={2025}, pages={1118-1123},
      keywords={Protocols;Linux;C++ languages;System integration;
      Writing;Manipulators;Real-time systems;Libraries;Timing;MATLAB},
      doi={10.1109/SII59315.2025.10871000}}


.. toctree::
   :hidden:
   :maxdepth: 3
   :caption: Table of Contents
   :glob:

   pages/getting_started/index
   pages/examples/index
   faq/faq
   guides/guides
   api/api
