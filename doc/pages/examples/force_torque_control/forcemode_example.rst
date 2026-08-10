Force Mode
==========

.. raw:: html

   <div style="width:60%; max-width:512px; cursor:pointer; border-radius:6px; overflow:hidden;"
        title="Click to pause / resume"
        onclick="var v=this.querySelector('video'); v.paused ? v.play() : v.pause();">
     <video autoplay loop muted playsinline
            style="width:100%; display:block; border-radius:6px;">
       <source src="../../../_static/forcemode_spiral_search.webm" type="video/webm">
     </video>
   </div>

.. role:: bash(code)
   :language: bash

.. role:: cmake_inline(code)
   :language: cmake

This example performs a spiral force-controlled search using **forceMode**. The robot moves in an
outward spiral in the XY plane while applying a downward force in Z, useful for peg-in-hole insertion tasks.

.. tabs::

   .. tab:: Python

      .. literalinclude:: ../../../../examples/py/forcemode_example.py
         :language: python

   .. tab:: C++

      .. literalinclude:: ../../../../examples/cpp/forcemode_example.cpp
         :language: c++

