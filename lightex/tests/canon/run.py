#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import unicode_literals

import os
import unittest
from os.path import join

import pylightex

CANONIZE = False

ROOT = join(os.path.dirname(os.path.realpath(__file__)), 'data')


class TestTeXtoHTML(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.olymp_sty = cls._load_static_file('olymp.sty')
        cls.template = cls._load_static_file('template.html')

    def _assert_files_equal(self, output, canon):
        output_lines = self._load_test_file(output).splitlines()
        canon_lines = self._load_test_file(canon).splitlines()
        self.assertEqual(len(output_lines), len(canon_lines))
        for output_line, canon_line in zip(output_lines, canon_lines):
            self.assertEqual(output_line, canon_line)

    def _make_context(self):
        ctxt = pylightex.Context()
        result = ctxt.execute(self.olymp_sty.encode('utf-8'))
        self.assertTrue(result.ok)
        return ctxt

    @staticmethod
    def _load_static_file(filename):
        with open(join(ROOT, 'static', filename), 'rb') as fd:
            return fd.read().decode('utf-8')

    @staticmethod
    def _load_test_file(filename):
        with open(join(ROOT, filename), 'rb') as fd:
            return fd.read().decode('utf-8')

    @staticmethod
    def _save_test_file(filename, data):
        with open(join(ROOT, filename), 'wb') as fd:
            fd.write(data.encode('utf-8'))

    def perform(self, name):
        inp = self._load_test_file('{0}.tex'.format(name))

        log_filename = '{0}.log'.format(name)
        output_filename = '{0}.output.html'.format(name)
        canon_filename = '{0}.canon.html'.format(name)

        ctxt = self._make_context()
        result = ctxt.execute(inp.encode('utf-8'))

        html = self.template.format(title=name, content=result.html)

        self._save_test_file(log_filename, result.error)

        if CANONIZE:
            self._save_test_file(canon_filename, html)
        else:
            self._save_test_file(output_filename, html)
            self._assert_files_equal(output_filename, canon_filename)

    # Test Cases

    def test_trees_1(self):
        self.perform('trees-1')

    def test_spanning_tree(self):
        self.perform('spanning-tree')

    def test_isomorph(self):
        self.perform('isomorph')

    def test_inversion(self):
        self.perform('inversion')

    def test_shooting(self):
        self.perform('shooting')

    def test_misc(self):
        self.perform('misc')

if __name__ == '__main__':
    unittest.main()
