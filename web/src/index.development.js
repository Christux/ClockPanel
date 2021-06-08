import remote from './remote';
import './style.scss';

remote.module('fakeServer', ['$httpInterpector', ($httpInterpector) => {

	const state = {
		color: {
			red: 43,
			green: 0,
			blue: 43
		},
		animation: 4,
		separator: 0,
		mirror: false
	};

	const settings = {
		timeZone: 1
	};

	const updateState = (newState) => {

		if (newState.hasOwnProperty('color')) {
			state.color = newState.color;
		}

		if (newState.hasOwnProperty('animation')) {
			state.animation = newState.animation;
		}

		if (newState.hasOwnProperty('separator')) {
			state.separator = newState.separator;
		}

		if (newState.hasOwnProperty('mirror')) {
			state.mirror = newState.mirror;
		}
	};

	const updateSettings = (newSettings) => {
		if (newSettings.hasOwnProperty('timeZone')) {
			settings.timeZone = newSettings.timeZone;
		}
	};

	$httpInterpector.add('GET', '/api', () => state);
	$httpInterpector.add('POST', '/api', (obj) => {
		updateState(obj);
		return state;
	});

	$httpInterpector.add('GET', '/settings', () => settings);
	$httpInterpector.add('POST', '/settings', obj => {
		updateSettings(obj);
		return settings;
	});

	$httpInterpector.add('GET', '/info', () => ({
		vendor: "Christux",
		model: "ClockPanel01",
		version: 1,
		serial: "PROTO01",
		firmware_build_date: "Aug 18 2017 23:42:24",
		ip_adress: "192.168.0.42",
		mac_adress: "6C:BF:9R:99:A2:86",
		Free_Heap: 37664,
		Flash_Chip_Size: 4194304,
		Flash_Chip_Frequency: 40000000
	}));

	return {};

}], true).boot();
