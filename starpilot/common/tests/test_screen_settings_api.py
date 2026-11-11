"""Exercise the real Galaxy settings handler with an in-memory Params store."""
import ast
from pathlib import Path

from flask import Flask, jsonify, request
import pytest

from test_screen_settings import Params


def client_for(params):
  source = Path(__file__).resolve().parents[3] / 'starpilot/system/the_galaxy/the_galaxy.py'
  setup = next(n for n in ast.parse(source.read_text()).body if isinstance(n, ast.FunctionDef) and n.name == 'setup')
  route = next(n for n in setup.body if isinstance(n, ast.FunctionDef) and n.name == 'get_param')
  app = Flask(__name__)
  env = dict(app=app, request=request, jsonify=jsonify, params=params, update_starpilot_toggles=lambda: None,
             LONGITUDINAL_MODE_KEYS=set(), PERSONALITY_PROFILES_PARAM='', PERSONALITY_PARKED_PARAM_KEYS=set(),
             PERSONALITY_PROFILE_ENABLE_PARAM_KEYS=set(), FAVORITE_SLOTS_PARAM='', MODEL_SMOOTHING_KEYS=set(),
             PERSONALITY_ADVANCED_PARAM_KEYS=set(), PERSONALITY_FOLLOW_PARAM_KEYS=set(),
             _get_param_type_info=lambda: (set(), {}))
  exec(compile(ast.Module(body=[route], type_ignores=[]), str(source), 'exec'), env)
  return app.test_client()


def test_mode_write_returns_remembered_manual_value():
  params = Params({'ScreenBrightnessOnroad': 32})
  response = client_for(params).put('/api/params', json={'key': 'ScreenBrightnessOnroad', 'value': 101})
  assert response.status_code == 200
  assert response.json['updated'] == {'ScreenBrightnessOnroad': 101, 'ScreenBrightnessOnroadManual': 32}


@pytest.mark.parametrize('key,value,expected', [('ScreenBrightnessOffset', -25, -25), ('ScreenBrightnessOnroad', 0, 0),
                                               ('ScreenManagement', True, True), ('ScreenManagement', False, False),
                                               ('StandbyWakeCriticalAlert', False, False),
                                               ('ScreenGazeDimming', True, True), ('ScreenGazeDimPercent', 35, 35),
                                               ('ScreenGazeDimDelay', 8, 8), ('ScreenGazeScreenSide', 1, 1),
                                               ('ScreenGazeYawThreshold', 10, 10)])
def test_screen_values_are_validated_and_return_typed_readback(key, value, expected):
  params = Params()
  response = client_for(params).put('/api/params', json={'key': key, 'value': value})
  assert response.status_code == 200
  assert response.json['updated'][key] == expected
  assert type(response.json['updated'][key]) is type(expected)
  assert params.values[key] == expected
  assert type(params.values[key]) is type(expected)


@pytest.mark.parametrize('key,value', [
  ('ScreenBrightnessOffset', 31), ('ScreenBrightnessOnroadOffset', -31), ('StandbyWakeEngage', 'false'), ('StandbyWakeFake', True),
  ('ScreenManagement', 'false'), ('ScreenManagement', 0), ('ScreenManagement', 1), ('ScreenManagement', None),
  ('ScreenGazeDimming', 'false'), ('ScreenGazeDimPercent', 0), ('ScreenGazeDimDelay', 31),
  ('ScreenGazeScreenSide', 2), ('ScreenGazeYawThreshold', 46), ('ScreenGazeUnknown', True),
])
def test_invalid_screen_requests_return_400_without_writes(key, value):
  params = Params()
  response = client_for(params).put('/api/params', json={'key': key, 'value': value})
  assert response.status_code == 400
  assert not params.values


def test_native_write_failure_is_reported_instead_of_success():
  params = Params()
  params.fail_key = 'ScreenBrightnessOffset'
  response = client_for(params).put('/api/params', json={'key': 'ScreenBrightnessOffset', 'value': 10})
  assert response.status_code == 503


@pytest.mark.parametrize('key', ['StandbyWakeCriticalAlert', 'ScreenManagement'])
def test_galaxy_failed_toggle_write_reports_error_and_restores_selection(key):
  from test_screen_settings_transactions import SilentOnceParams, galaxy_params
  params = SilentOnceParams({key: True})
  response = client_for(galaxy_params(params)).put('/api/params', json={'key': key, 'value': False})
  assert response.status_code == 503
  assert params.values == {key: True}
