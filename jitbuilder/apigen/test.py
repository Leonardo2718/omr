#! /usr/bin/env python

from jsonschema import Draft6Validator, validate
import json
import unittest

class ValidateSchemas(unittest.TestCase):

    def test_api_schema(self):
        with open('api.schema.json') as f:
            schema = json.loads(f.read())
            Draft6Validator.check_schema(schema)

    def test_api_type_schema(self):
        with open('api-type.schema.json') as f:
            schema = json.loads(f.read())
            Draft6Validator.check_schema(schema)

    def test_api_field_schema(self):
        with open('api-field.schema.json') as f:
            schema = json.loads(f.read())
            Draft6Validator.check_schema(schema)

    def test_api_service_schema(self):
        with open('api-service.schema.json') as f:
            schema = json.loads(f.read())
            Draft6Validator.check_schema(schema)

    def test_api_class_schema(self):
        with open('api-class.schema.json') as f:
            schema = json.loads(f.read())
            Draft6Validator.check_schema(schema)

if __name__ == '__main__':
    unittest.main()
