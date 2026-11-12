"""Startup hooks for the isolated build-tool overlay, never device artifacts.

Python does not execute .pth files in a PYTHONPATH-only directory. Acados still
uses the future_fstrings source encoding, whose wheel normally registers its
codec from such a .pth file; register it when each build child starts instead.
"""
import future_fstrings

future_fstrings.register()
