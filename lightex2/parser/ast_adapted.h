#pragma once

#include <list>
#include <string>

#include "ast.h"

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/optional/optional.hpp>

BOOST_FUSION_ADAPT_STRUCT(lightex2::ast::Body, nodes);

BOOST_FUSION_ADAPT_STRUCT(lightex2::ast::Paragraph, nodes);

BOOST_FUSION_ADAPT_STRUCT(lightex2::ast::PlainText, text);

BOOST_FUSION_ADAPT_STRUCT(lightex2::ast::VerbatimText, text);

BOOST_FUSION_ADAPT_STRUCT(lightex2::ast::Bold, nodes);
BOOST_FUSION_ADAPT_STRUCT(lightex2::ast::Italic, nodes);
BOOST_FUSION_ADAPT_STRUCT(lightex2::ast::Typewriter, nodes);
BOOST_FUSION_ADAPT_STRUCT(lightex2::ast::Emphasis, nodes);

BOOST_FUSION_ADAPT_STRUCT(lightex2::ast::Symbol, value);

BOOST_FUSION_ADAPT_STRUCT(lightex2::ast::Group, nodes);

BOOST_FUSION_ADAPT_STRUCT(lightex2::ast::Example, input, output);
BOOST_FUSION_ADAPT_STRUCT(lightex2::ast::ExampleTable, examples);
